//
// Created by quepas on 23/02/2020.
//

#include "PLYLoader.hpp"

#include <filesystem>
#include <iostream>
#include <rplyfile.h>

namespace fs = std::filesystem;
using std::cerr;
using std::endl;
using std::string;
using std::vector;

bool PLYLoader::LoadFromPath(const string &path) {
    if (path.empty()) {
        cerr << "Empty path" << endl;
        return false;
    }
    if (!fs::exists(path)) {
        cerr << "Non-existing path=" << path << endl;
        return false;
    }
    p_ply ply = ply_open(path.c_str(), nullptr, 0, nullptr);
    return LoadPLY(ply);
}

bool PLYLoader::LoadFromDescriptor(FILE *fd) {
    if (!fd) {
        cerr << "Invalid file descriptor" << endl;
        return false;
    }
    p_ply ply = ply_open_from_file(fd, nullptr, 0, nullptr);
    return LoadPLY(ply);
}

// RPly callbacks
int vertex_cb(p_ply_argument argument) {
    PLYLoader::VertexConsumer *that;
    long property_position;
    ply_get_argument_user_data(argument, reinterpret_cast<void **>(&that), &property_position);
    that->Eat(ply_get_argument_value(argument));
    return 1;
}

int colour_cb(p_ply_argument argument) {
    PLYLoader::ColourConsumer *that;
    long property_position;
    ply_get_argument_user_data(argument, reinterpret_cast<void **>(&that), &property_position);
    that->Eat(static_cast<unsigned char>(ply_get_argument_value(argument)));
    return 1;
}

int normal_cb(p_ply_argument argument) {
    PLYLoader::NormalConsumer *that;
    long property_position;
    ply_get_argument_user_data(argument, reinterpret_cast<void **>(&that), &property_position);
    that->Eat(ply_get_argument_value(argument));
    return 1;
}

int face_cb(p_ply_argument argument) {
    PLYLoader::FaceConsumer *that;
    long length, value_index;
    ply_get_argument_property(argument, nullptr, &length, &value_index);
    ply_get_argument_user_data(argument, reinterpret_cast<void **>(&that), nullptr);
    if (value_index >= 0) {
        that->Eat(static_cast<unsigned int>(ply_get_argument_value(argument)));
    }
    return 1;
}


bool PLYLoader::LoadPLY(p_ply &ply) {
    if (!ply) {
        cerr << "Wrong file" << endl;
        return false;
    }
    if (!ply_read_header(ply)) {
        cerr << "Wrong header. Not a PLY file" << endl;
        ply_close(ply);
        return false;
    }
    // At this point, we need to remove the previously loaded model
    Clear();
    // Read vertices
    DataConsumer<Vertex, 3, float> vertexConsumer(vertices);
    long num_verts = ply_set_read_cb(ply, "vertex", "x", vertex_cb, &vertexConsumer, 0);
    ply_set_read_cb(ply, "vertex", "y", vertex_cb, &vertexConsumer, 1);
    ply_set_read_cb(ply, "vertex", "z", vertex_cb, &vertexConsumer, 2);
    vertices.reserve(num_verts);
    // Read colors
    ColourConsumer colourConsumer(colours);
    long num_colors = ply_set_read_cb(ply, "vertex", "red", colour_cb, &colourConsumer, 0);
    ply_set_read_cb(ply, "vertex", "green", colour_cb, &colourConsumer, 1);
    ply_set_read_cb(ply, "vertex", "blue", colour_cb, &colourConsumer, 2);
    colours.reserve(num_colors);
    // Read normals
    NormalConsumer normalConsumer(normals);
    long num_normals = ply_set_read_cb(ply, "vertex", "nx", normal_cb, &normalConsumer, 0);
    ply_set_read_cb(ply, "vertex", "ny", normal_cb, &normalConsumer, 1);
    ply_set_read_cb(ply, "vertex", "nz", normal_cb, &normalConsumer, 2);
    normals.reserve(num_normals);
    // Read faces
    FaceConsumer faceConsumer(faces);
    long num_faces = ply_set_read_cb(ply, "face", "vertex_indices", face_cb, &faceConsumer, 0);
    faces.reserve(num_faces);

    if (!ply_read(ply)) {
        cerr << "Failed reading PLY file" << endl;
        ply_close(ply);
        return false;
    }
    if (!ply_close(ply)) {
        cerr << "Failed closing PLY file" << endl;
        return false;
    }
    return true;
}

void PLYLoader::Clear() {
    vertices.clear();
    normals.clear();
    colours.clear();
    faces.clear();
}

template <typename P, int N, typename F>
void PLYLoader::DataConsumer<P, N, F>::Eat(F coordinate) {
    int max = N;
    int idx = counter % max;
    values[idx] = coordinate;
    if (idx == (max-1)) {
        vertices.push_back(P{values});
    }
    ++counter;
}

template<typename P, int N, typename F>
PLYLoader::DataConsumer<P, N, F>::DataConsumer(vector<P> &vertices):vertices(vertices) {}

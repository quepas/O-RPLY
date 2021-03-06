//
// Created by quepas on 23/02/2020.
//

#include "PLYLoader.hpp"

#include <filesystem>
#include <iostream>
#include <rplyfile.h>
#include <rplymemory.h>

namespace fs = std::filesystem;
using std::cerr;
using std::endl;
using std::string;
using std::vector;

#define CHECK_FOR_ERRORS(cond) if (!cond) return false;

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

bool PLYLoader::LoadFromMemory(const char *buffer) {
    if (!buffer) {
        cerr << "Empty buffer" << endl;
        return false;
    }
    p_ply ply = ply_open_from_memory(buffer, nullptr, 0, nullptr);
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
    DataConsumer<Vertex, 3, float> vertex_consumer(vertices);
    long num_verts = ply_set_read_cb(ply, "vertex", "x", vertex_cb, &vertex_consumer, 0);
    ply_set_read_cb(ply, "vertex", "y", vertex_cb, &vertex_consumer, 1);
    ply_set_read_cb(ply, "vertex", "z", vertex_cb, &vertex_consumer, 2);
    vertices.reserve(num_verts);
    // Read colors
    ColourConsumer colour_consumer(colours);
    long num_colors = ply_set_read_cb(ply, "vertex", "red", colour_cb, &colour_consumer, 0);
    ply_set_read_cb(ply, "vertex", "green", colour_cb, &colour_consumer, 1);
    ply_set_read_cb(ply, "vertex", "blue", colour_cb, &colour_consumer, 2);
    colours.reserve(num_colors);
    // Read normals
    NormalConsumer normal_consumer(normals);
    long num_normals = ply_set_read_cb(ply, "vertex", "nx", normal_cb, &normal_consumer, 0);
    ply_set_read_cb(ply, "vertex", "ny", normal_cb, &normal_consumer, 1);
    ply_set_read_cb(ply, "vertex", "nz", normal_cb, &normal_consumer, 2);
    normals.reserve(num_normals);
    // Read faces
    FaceConsumer face_consumer(faces);
    long num_faces = ply_set_read_cb(ply, "face", "vertex_indices", face_cb, &face_consumer, 0);
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

bool PLYLoader::StoreToPath(const string &path, bool binary_format) {
    if (NumVertices() == 0) {
        cerr << "No vertices to write" << endl;
        return false;
    }
    if (path.empty()) {
        cerr << "Empty filename" << endl;
        return false;
    }
    p_ply ply = ply_create(path.c_str(), binary_format ? PLY_LITTLE_ENDIAN : PLY_ASCII, nullptr, 0, nullptr);
    return StorePLY(ply);
}

bool PLYLoader::StoreToDescriptor(FILE *fd, bool binary_format) {
    if (NumVertices() == 0) {
        cerr << "No vertices to write" << endl;
        return false;
    }
    p_ply ply = ply_create_to_file(fd, binary_format ? PLY_LITTLE_ENDIAN : PLY_ASCII, nullptr, 0, nullptr);
    return StorePLY(ply);
}

bool PLYLoader::StoreToMemory(char *&buffer, size_t &ply_size, bool binary_format, size_t max_buffer_size /* 5MB */) {
    buffer = new char[max_buffer_size];
    p_ply ply = ply_create_to_memory(buffer, max_buffer_size, &ply_size, binary_format ? PLY_LITTLE_ENDIAN : PLY_ASCII,
                                     nullptr, 0,
                                     nullptr);
    return StorePLY(ply);
}

bool PLYLoader::StorePLY(p_ply &ply) {
    if (!ply) {
        cerr << "Invalid PLY handler" << endl;
        return false;
    }
    size_t num_vertices = NumVertices();
    bool write_colors = (NumColours() == num_vertices);
    bool write_normals = (NumNormals() == num_vertices);

    if (ply_add_element(ply, "vertex", num_vertices)) {
        CHECK_FOR_ERRORS(ply_add_scalar_property(ply, "x", PLY_FLOAT));
        CHECK_FOR_ERRORS(ply_add_scalar_property(ply, "y", PLY_FLOAT));
        CHECK_FOR_ERRORS(ply_add_scalar_property(ply, "z", PLY_FLOAT));
    }
    if (write_colors) {
        CHECK_FOR_ERRORS(ply_add_scalar_property(ply, "red", PLY_UCHAR));
        CHECK_FOR_ERRORS(ply_add_scalar_property(ply, "green", PLY_UCHAR));
        CHECK_FOR_ERRORS(ply_add_scalar_property(ply, "blue", PLY_UCHAR));
    }
    if (write_normals) {
        CHECK_FOR_ERRORS(ply_add_scalar_property(ply, "nx", PLY_FLOAT));
        CHECK_FOR_ERRORS(ply_add_scalar_property(ply, "ny", PLY_FLOAT));
        CHECK_FOR_ERRORS(ply_add_scalar_property(ply, "nz", PLY_FLOAT));
    }
    if (NumFaces() > 0) {
        if (ply_add_element(ply, "face", NumFaces())) {
            CHECK_FOR_ERRORS(ply_add_list_property(ply, "vertex_indices", PLY_UCHAR, PLY_INT));
        }
    }
    CHECK_FOR_ERRORS(ply_add_comment(ply, "PLYLoader"));
    CHECK_FOR_ERRORS(ply_add_obj_info(ply, "Generated using o-rply? and PLYLoader"));

    if (!ply_write_header(ply)) {
        cerr << "Error writing file header" << endl;
        if (!ply_close(ply)) {
            cerr << "Failed at closing the file" << endl;
        }
        return false;
    }

    for (size_t index = 0; index < num_vertices; ++index) {
        auto vertex = vertices[index];
        CHECK_FOR_ERRORS(ply_write(ply, vertex.x));
        CHECK_FOR_ERRORS(ply_write(ply, vertex.y));
        CHECK_FOR_ERRORS(ply_write(ply, vertex.z));

        if (write_colors) {
            auto color = colours[index];
            CHECK_FOR_ERRORS(ply_write(ply, color.red));
            CHECK_FOR_ERRORS(ply_write(ply, color.green));
            CHECK_FOR_ERRORS(ply_write(ply, color.blue));
        }
        if (write_normals) {
            auto normal = normals[index];
            CHECK_FOR_ERRORS(ply_write(ply, normal.nx));
            CHECK_FOR_ERRORS(ply_write(ply, normal.ny));
            CHECK_FOR_ERRORS(ply_write(ply, normal.nz));
        }
    }
    for (size_t index = 0; index < NumFaces(); ++index) {
        auto face = faces[index];
        CHECK_FOR_ERRORS(ply_write(ply, 3));
        CHECK_FOR_ERRORS(ply_write(ply, face.vertex_indices[0]));
        CHECK_FOR_ERRORS(ply_write(ply, face.vertex_indices[1]));
        CHECK_FOR_ERRORS(ply_write(ply, face.vertex_indices[2]));
    }
    if (!ply_close(ply)) {
        cerr << "Failed at closing the file" << endl;
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

template<typename P, int N, typename F>
void PLYLoader::DataConsumer<P, N, F>::Eat(F coordinate) {
    int max = N;
    int idx = counter % max;
    values[idx] = coordinate;
    if (idx == (max - 1)) {
        vertices.push_back(P{values});
    }
    ++counter;
}

template<typename P, int N, typename F>
PLYLoader::DataConsumer<P, N, F>::DataConsumer(vector<P> &vertices):vertices(vertices) {}

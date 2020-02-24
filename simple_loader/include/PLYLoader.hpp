//
// Created by quepas on 23/02/2020.
//

#ifndef LIBRPLY_PLYLOADER_HPP
#define LIBRPLY_PLYLOADER_HPP

#include <string>
#include <vector>
#include <rply.h>

class PLYLoader {

public:

    struct Vertex {
        Vertex(const float data[3]) : x(data[0]), y(data[1]), z(data[2]) {}
        float x, y, z;
    };

    struct Normal {
        Normal(const float data[3]) : nx(data[0]), ny(data[1]), nz(data[2]) {}
        float nx, ny, nz;
    };

    struct Colour {
        Colour(const unsigned char data[3]) : red(data[0]), green(data[1]), blue(data[2]) {}
        unsigned char red, green, blue;
    };

    struct Face {
        Face(const unsigned int data[3]) : vertex_indices{data[0], data[1], data[2]} {}
        unsigned int vertex_indices[3];
    };


    bool LoadFromPath(const std::string& path);
    bool LoadFromDescriptor(FILE* fd);
    bool LoadFromMemory(const char* buffer);

    bool StoreToPath(const std::string &path, bool binary_format);
    bool StoreToDescriptor(FILE* fd, bool binary_format);
    bool StoreToMemory(char *&buffer, size_t &ply_size, bool binary_format, size_t max_buffer_size = 5 * 1024 * 1024 /* 5MB */);

    size_t NumVertices() { return vertices.size(); }
    size_t NumNormals() { return normals.size(); }
    size_t NumColours() { return colours.size(); }
    size_t NumFaces() { return faces.size(); }

    void Clear();

    template <typename P, int N, typename F>
    class DataConsumer {
    public:
        DataConsumer(std::vector<P> &vertices);
        void Eat(F value);
    private:
        std::vector<P>& vertices;
        F values[N];
        int counter = 0;
    };

    typedef DataConsumer<Vertex, 3, float> VertexConsumer;
    typedef DataConsumer<Normal, 3, float> NormalConsumer;
    typedef DataConsumer<Colour, 3, unsigned char> ColourConsumer;
    typedef DataConsumer<Face, 3, unsigned int> FaceConsumer;

private:
    std::vector<Vertex> vertices;
    std::vector<Normal> normals;
    std::vector<Colour> colours;
    std::vector<Face> faces;

    bool LoadPLY(p_ply& ply);
    bool StorePLY(p_ply& ply);

};

#endif //LIBRPLY_PLYLOADER_HPP

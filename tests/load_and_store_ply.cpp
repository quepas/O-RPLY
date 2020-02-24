//
// Created by quepas on 23/02/2020.
//

#define CATCH_CONFIG_MAIN

#include "PLYLoader.hpp"
#include <catch2/catch.hpp>
#include <iostream>

using std::cerr;
using std::endl;
using std::string;

const char* LoadPLYFileAsBuffer(const string& file_path) {
    FILE *fd = fopen(file_path.c_str(), "rb");
    if (!fd) {
        cerr << "Wrong file" << endl;
        return nullptr;
    }
    fseek(fd, 0, SEEK_END);
    long fsize = ftell(fd);
    if (fsize == 0) {
        cerr << "The file is empty" << endl;
        return nullptr;
    }
    fseek(fd, 0, SEEK_SET);
    char *buffer = (char *)malloc(fsize + 1);
    fread(buffer, fsize, 1, fd);
    fclose(fd);
    return buffer;
}

TEST_CASE("Loading PLY files from disc", "[load_ply_files]") {

    PLYLoader ply_loader;

    SECTION("Load cow.ply from a path (ascii)") {
        REQUIRE(ply_loader.LoadFromPath("../ply_files/cow.ply"));
        REQUIRE(ply_loader.NumVertices() == 2903);
        REQUIRE(ply_loader.NumNormals() == 0);
        REQUIRE(ply_loader.NumColours() == 0);
        REQUIRE(ply_loader.NumFaces() == 5804);
    }

    SECTION("Load airplane.ply from a path (ascii)") {
        REQUIRE(ply_loader.LoadFromPath("../ply_files/airplane.ply"));
        REQUIRE(ply_loader.NumVertices() == 1335);
        REQUIRE(ply_loader.NumNormals() == 0);
        REQUIRE(ply_loader.NumColours() == 0);
        REQUIRE(ply_loader.NumFaces() == 2452);
    }

    SECTION("Load cow.ply from a file descriptor (ascii)") {
        auto* fd = fopen("../ply_files/cow.ply", "r");
        REQUIRE(fd != nullptr);
        REQUIRE(ply_loader.LoadFromDescriptor(fd));
        REQUIRE(!fclose(fd));
        REQUIRE(ply_loader.NumVertices() == 2903);
        REQUIRE(ply_loader.NumNormals() == 0);
        REQUIRE(ply_loader.NumColours() == 0);
        REQUIRE(ply_loader.NumFaces() == 5804);
    }

    SECTION("Load airplane.ply from a file descriptor (ascii)") {
        auto* fd = fopen("../ply_files/airplane.ply", "r");
        REQUIRE(fd != nullptr);
        REQUIRE(ply_loader.LoadFromDescriptor(fd));
        REQUIRE(!fclose(fd));
        REQUIRE(ply_loader.NumVertices() == 1335);
        REQUIRE(ply_loader.NumNormals() == 0);
        REQUIRE(ply_loader.NumColours() == 0);
        REQUIRE(ply_loader.NumFaces() == 2452);
    }
}

TEST_CASE("Loading PLY files from memory", "[load_ply_files_memory]") {

    PLYLoader ply_loader, ply_store;

    SECTION("Load cow.ply from memory (ascii)") {
        const char* buffer = LoadPLYFileAsBuffer("../ply_files/cow.ply");
        REQUIRE(buffer != nullptr);
        REQUIRE(ply_loader.LoadFromMemory(buffer));
        delete[] buffer;
        REQUIRE(ply_loader.NumVertices() == 2903);
        REQUIRE(ply_loader.NumNormals() == 0);
        REQUIRE(ply_loader.NumColours() == 0);
        REQUIRE(ply_loader.NumFaces() == 5804);
    }

    SECTION("Load airplane.ply from memory (ascii)") {
        const char* buffer = LoadPLYFileAsBuffer("../ply_files/airplane.ply");
        REQUIRE(buffer != nullptr);
        REQUIRE(ply_loader.LoadFromMemory(buffer));
        REQUIRE(ply_loader.NumVertices() == 1335);
        REQUIRE(ply_loader.NumNormals() == 0);
        REQUIRE(ply_loader.NumColours() == 0);
        REQUIRE(ply_loader.NumFaces() == 2452);
    }

    SECTION("Load cow.ply from memory (binary)") {
        REQUIRE(ply_store.LoadFromPath("../ply_files/cow.ply"));
        REQUIRE(ply_store.StoreToPath("cow_binary.ply", true));
        const char* buffer = LoadPLYFileAsBuffer("cow_binary.ply");
        REQUIRE(buffer != nullptr);
        REQUIRE(ply_loader.LoadFromMemory(buffer));
        delete[] buffer;
        REQUIRE(ply_loader.NumVertices() == 2903);
        REQUIRE(ply_loader.NumNormals() == 0);
        REQUIRE(ply_loader.NumColours() == 0);
        REQUIRE(ply_loader.NumFaces() == 5804);
    }

    SECTION("Load airplane.ply from memory (binary)") {
        REQUIRE(ply_store.LoadFromPath("../ply_files/airplane.ply"));
        REQUIRE(ply_store.StoreToPath("airplane_binary.ply", true));
        const char* buffer = LoadPLYFileAsBuffer("airplane_binary.ply");
        REQUIRE(buffer != nullptr);
        REQUIRE(ply_loader.LoadFromMemory(buffer));
        REQUIRE(ply_loader.NumVertices() == 1335);
        REQUIRE(ply_loader.NumNormals() == 0);
        REQUIRE(ply_loader.NumColours() == 0);
        REQUIRE(ply_loader.NumFaces() == 2452);
    }

}

TEST_CASE("Storing PLY files on disc", "[store_ply_files]") {

    PLYLoader ply_store, ply_loader;

    SECTION("Store cow.ply as ascii file") {
        REQUIRE(ply_store.LoadFromPath("../ply_files/cow.ply"));
        REQUIRE(ply_store.StoreToPath("cow_ascii.ply", false));
        REQUIRE(ply_loader.LoadFromPath("cow_ascii.ply"));
        REQUIRE(ply_loader.NumVertices() == 2903);
        REQUIRE(ply_loader.NumNormals() == 0);
        REQUIRE(ply_loader.NumColours() == 0);
        REQUIRE(ply_loader.NumFaces() == 5804);
    }

    SECTION("Store cow.ply as ascii file with file descriptor") {
        REQUIRE(ply_store.LoadFromPath("../ply_files/cow.ply"));
        auto* fd = fopen("cow_ascii_fd.ply", "w");
        REQUIRE(fd != nullptr);
        REQUIRE(ply_store.StoreToDescriptor(fd, false));
        REQUIRE(!fclose(fd));
        REQUIRE(ply_loader.LoadFromPath("cow_ascii_fd.ply"));
        REQUIRE(ply_loader.NumVertices() == 2903);
        REQUIRE(ply_loader.NumNormals() == 0);
        REQUIRE(ply_loader.NumColours() == 0);
        REQUIRE(ply_loader.NumFaces() == 5804);
    }

    SECTION("Store cow.ply as binary file") {
        REQUIRE(ply_store.LoadFromPath("../ply_files/cow.ply"));
        REQUIRE(ply_store.StoreToPath("cow_binary.ply", true));
        REQUIRE(ply_loader.LoadFromPath("cow_binary.ply"));
        REQUIRE(ply_loader.NumVertices() == 2903);
        REQUIRE(ply_loader.NumNormals() == 0);
        REQUIRE(ply_loader.NumColours() == 0);
        REQUIRE(ply_loader.NumFaces() == 5804);
    }

    SECTION("Store airplane.ply as ascii file") {
        REQUIRE(ply_store.LoadFromPath("../ply_files/airplane.ply"));
        REQUIRE(ply_store.StoreToPath("airplane_ascii.ply", false));
        REQUIRE(ply_loader.LoadFromPath("airplane_ascii.ply"));
        REQUIRE(ply_loader.NumVertices() == 1335);
        REQUIRE(ply_loader.NumNormals() == 0);
        REQUIRE(ply_loader.NumColours() == 0);
        REQUIRE(ply_loader.NumFaces() == 2452);
    }

    SECTION("Store airplane.ply as binary file") {
        REQUIRE(ply_store.LoadFromPath("../ply_files/airplane.ply"));
        REQUIRE(ply_store.StoreToPath("airplane_binary.ply", true));
        REQUIRE(ply_loader.LoadFromPath("airplane_binary.ply"));
        REQUIRE(ply_loader.NumVertices() == 1335);
        REQUIRE(ply_loader.NumNormals() == 0);
        REQUIRE(ply_loader.NumColours() == 0);
        REQUIRE(ply_loader.NumFaces() == 2452);
    }

    SECTION("Store airplane.ply as binary file with file descriptor") {
        REQUIRE(ply_store.LoadFromPath("../ply_files/airplane.ply"));
        auto* fd = fopen("airplane_binary_fd.ply", "w");
        REQUIRE(fd != nullptr);
        REQUIRE(ply_store.StoreToDescriptor(fd, true));
        REQUIRE(!fclose(fd));
        REQUIRE(ply_loader.LoadFromPath("airplane_binary_fd.ply"));
        REQUIRE(ply_loader.NumVertices() == 1335);
        REQUIRE(ply_loader.NumNormals() == 0);
        REQUIRE(ply_loader.NumColours() == 0);
        REQUIRE(ply_loader.NumFaces() == 2452);
    }
}

TEST_CASE("Storing PLY files in memory", "[store_ply_files_memory]") {

    PLYLoader ply_store, ply_loader;

    SECTION("Store cow.ply in memory as ascii") {
        REQUIRE(ply_store.LoadFromPath("../ply_files/cow.ply"));
        char* buffer;
        size_t ply_size;
        REQUIRE(ply_store.StoreToMemory(buffer, ply_size, false));
        REQUIRE(ply_loader.LoadFromMemory(buffer));
        delete[] buffer;
        REQUIRE(ply_loader.NumVertices() == 2903);
        REQUIRE(ply_loader.NumNormals() == 0);
        REQUIRE(ply_loader.NumColours() == 0);
        REQUIRE(ply_loader.NumFaces() == 5804);
    }

    SECTION("Store cow.ply in memory as binary") {
        ply_store.Clear();
        REQUIRE(ply_store.LoadFromPath("../ply_files/cow.ply"));
        char* buffer;
        size_t ply_size;
        REQUIRE(ply_store.StoreToMemory(buffer, ply_size, true));
        REQUIRE(ply_loader.LoadFromMemory(buffer));
        delete[] buffer;
        REQUIRE(ply_loader.NumVertices() == 2903);
        REQUIRE(ply_loader.NumNormals() == 0);
        REQUIRE(ply_loader.NumColours() == 0);
        REQUIRE(ply_loader.NumFaces() == 5804);
    }

    SECTION("Store airplane.ply in memory as ascii") {
        REQUIRE(ply_store.LoadFromPath("../ply_files/airplane.ply"));
        char* buffer;
        size_t ply_size;
        REQUIRE(ply_store.StoreToMemory(buffer, ply_size, false));
        REQUIRE(ply_loader.LoadFromMemory(buffer));
        delete[] buffer;
        REQUIRE(ply_loader.NumVertices() == 1335);
        REQUIRE(ply_loader.NumNormals() == 0);
        REQUIRE(ply_loader.NumColours() == 0);
        REQUIRE(ply_loader.NumFaces() == 2452);
    }

    SECTION("Store airplane.ply in memory as binary") {
        REQUIRE(ply_store.LoadFromPath("../ply_files/airplane.ply"));
        char* buffer;
        size_t ply_size;
        REQUIRE(ply_store.StoreToMemory(buffer, ply_size, true));
        REQUIRE(ply_loader.LoadFromMemory(buffer));
        delete[] buffer;
        REQUIRE(ply_loader.NumVertices() == 1335);
        REQUIRE(ply_loader.NumNormals() == 0);
        REQUIRE(ply_loader.NumColours() == 0);
        REQUIRE(ply_loader.NumFaces() == 2452);
    }
}

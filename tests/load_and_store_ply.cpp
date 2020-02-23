//
// Created by quepas on 23/02/2020.
//

#define CATCH_CONFIG_MAIN

#include <PLYLoader.hpp>
#include "catch2/catch.hpp"

const char* LoadPLYFileAsBuffer(const std::string& file_path) {
    FILE *f = fopen(file_path.c_str(), "rb");
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *buffer = (char *)malloc(fsize + 1);
    fread(buffer, fsize, 1, f);
    fclose(f);
    return buffer;
}

TEST_CASE("Loading PLY files from disc", "[load_ply_files]") {

    PLYLoader ply_loader;

    SECTION("Load cow.ply from a path (ascii)") {
        ply_loader.LoadFromPath("../ply_files/cow.ply");
        REQUIRE(ply_loader.NumVertices() == 2903);
        REQUIRE(ply_loader.NumNormals() == 0);
        REQUIRE(ply_loader.NumColours() == 0);
        REQUIRE(ply_loader.NumFaces() == 5804);
    }

    SECTION("Load airplane.ply from a path (ascii)") {
        ply_loader.LoadFromPath("../ply_files/airplane.ply");
        REQUIRE(ply_loader.NumVertices() == 1335);
        REQUIRE(ply_loader.NumNormals() == 0);
        REQUIRE(ply_loader.NumColours() == 0);
        REQUIRE(ply_loader.NumFaces() == 2452);
    }

    SECTION("Load cow.ply from a file descriptor (ascii)") {
        auto* fd = fopen("../ply_files/cow.ply", "r");
        ply_loader.LoadFromDescriptor(fd);
        fclose(fd);
        REQUIRE(ply_loader.NumVertices() == 2903);
        REQUIRE(ply_loader.NumNormals() == 0);
        REQUIRE(ply_loader.NumColours() == 0);
        REQUIRE(ply_loader.NumFaces() == 5804);
    }

    SECTION("Load airplane.ply from a file descriptor (ascii)") {
        auto* fd = fopen("../ply_files/airplane.ply", "r");
        ply_loader.LoadFromDescriptor(fd);
        fclose(fd);
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
        ply_store.LoadFromPath("../ply_files/cow.ply");
        ply_store.StoreToPath("cow_binary.ply", true);
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
        ply_store.LoadFromPath("../ply_files/airplane.ply");
        ply_store.StoreToPath("airplane_binary.ply", true);
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
        ply_store.LoadFromPath("../ply_files/cow.ply");
        ply_store.StoreToPath("cow_ascii.ply", false);
        ply_loader.LoadFromPath("cow_ascii.ply");
        REQUIRE(ply_loader.NumVertices() == 2903);
        REQUIRE(ply_loader.NumNormals() == 0);
        REQUIRE(ply_loader.NumColours() == 0);
        REQUIRE(ply_loader.NumFaces() == 5804);
    }

    SECTION("Store cow.ply as ascii file with file descriptor") {
        ply_store.LoadFromPath("../ply_files/cow.ply");
        auto* fd = fopen("cow_ascii_fd.ply", "w");
        ply_store.StoreToDescriptor(fd, false);
        fclose(fd);
        ply_loader.LoadFromPath("cow_ascii_fd.ply");
        REQUIRE(ply_loader.NumVertices() == 2903);
        REQUIRE(ply_loader.NumNormals() == 0);
        REQUIRE(ply_loader.NumColours() == 0);
        REQUIRE(ply_loader.NumFaces() == 5804);
    }

    SECTION("Store cow.ply as binary file") {
        ply_store.LoadFromPath("../ply_files/cow.ply");
        ply_store.StoreToPath("cow_binary.ply", true);
        ply_loader.LoadFromPath("cow_binary.ply");
        REQUIRE(ply_loader.NumVertices() == 2903);
        REQUIRE(ply_loader.NumNormals() == 0);
        REQUIRE(ply_loader.NumColours() == 0);
        REQUIRE(ply_loader.NumFaces() == 5804);
    }

    SECTION("Store airplane.ply as ascii file") {
        ply_store.LoadFromPath("../ply_files/airplane.ply");
        ply_store.StoreToPath("airplane_ascii.ply", false);
        ply_loader.LoadFromPath("airplane_ascii.ply");
        REQUIRE(ply_loader.NumVertices() == 1335);
        REQUIRE(ply_loader.NumNormals() == 0);
        REQUIRE(ply_loader.NumColours() == 0);
        REQUIRE(ply_loader.NumFaces() == 2452);
    }

    SECTION("Store airplane.ply as binary file") {
        ply_store.LoadFromPath("../ply_files/airplane.ply");
        ply_store.StoreToPath("airplane_binary.ply", true);
        ply_loader.LoadFromPath("airplane_binary.ply");
        REQUIRE(ply_loader.NumVertices() == 1335);
        REQUIRE(ply_loader.NumNormals() == 0);
        REQUIRE(ply_loader.NumColours() == 0);
        REQUIRE(ply_loader.NumFaces() == 2452);
    }

    SECTION("Store airplane.ply as binary file with file descriptor") {
        ply_store.LoadFromPath("../ply_files/airplane.ply");
        auto* fd = fopen("airplane_binary_fd.ply", "w");
        ply_store.StoreToDescriptor(fd, true);
        fclose(fd);
        ply_loader.LoadFromPath("airplane_binary_fd.ply");
        REQUIRE(ply_loader.NumVertices() == 1335);
        REQUIRE(ply_loader.NumNormals() == 0);
        REQUIRE(ply_loader.NumColours() == 0);
        REQUIRE(ply_loader.NumFaces() == 2452);
    }
}

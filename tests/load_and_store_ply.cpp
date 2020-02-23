//
// Created by quepas on 23/02/2020.
//

#define CATCH_CONFIG_MAIN

#include <PLYLoader.hpp>
#include "catch2/catch.hpp"

TEST_CASE("Loading PLY files", "[load_ply_files]") {

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
        REQUIRE(ply_loader.NumVertices() == 2903);
        REQUIRE(ply_loader.NumNormals() == 0);
        REQUIRE(ply_loader.NumColours() == 0);
        REQUIRE(ply_loader.NumFaces() == 5804);
    }

    SECTION("Load airplane.ply from a file descriptor (ascii)") {
        auto* fd = fopen("../ply_files/airplane.ply", "r");
        ply_loader.LoadFromDescriptor(fd);
        REQUIRE(ply_loader.NumVertices() == 1335);
        REQUIRE(ply_loader.NumNormals() == 0);
        REQUIRE(ply_loader.NumColours() == 0);
        REQUIRE(ply_loader.NumFaces() == 2452);
    }
}

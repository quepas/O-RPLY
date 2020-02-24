# O RPLY?

_**O RPLY?**_ is a C/C++ library for PLY file format load and store. You might ask O RLY? YA RLY. It is built over [RPly 1.1.4 by Diego Nehab](http://w3.impa.br/~diego/software/rply/) with three improvements: 1) [CMake build](CMakeLists.txt) 2) in-memory load and store; and 3) implementation of a [simple loader for PLY files](simple_loader/). NO WAI!!  

This _petite library_ exists only due to the immense work of [Diego Nehab](http://w3.impa.br/~diego/). Thank you!

![Build (linux/gcc9)](https://github.com/quepas/O-RPLY/workflows/Build%20(linux/gcc9)/badge.svg?branch=master)

## What's new?

1. [CMake-based build](CMakeLists.txt) makes the library easy to build and reuse
2. Two new functions for in-memory load and store:
    ```cpp
   // Starts reading the PLY file from memory (const char[] buffer)
   p_ply ply_open_from_memory(const char *buffer, p_ply_error_cb error_cb, long idata, void *pdata);
   // Starts writing the PLY file to memory (pre-allocated char[] buffer)
   p_ply ply_create_to_memory(char *buffer, size_t buffer_size, size_t *ply_size,
                               e_ply_storage_mode storage_mode, p_ply_error_cb error_cb, long idata, void *pdata);
    ```
3. An implementation of a simple [PLYLoader class](simple_loader/include/PLYLoader.hpp).

## Installation

On Linux-based systems:

1. Clone this repository:
    ```shell script
    git clone https://github.com/quepas/O-RPLY.git
    ```
2. Pull dependencies (e.g. [Catch2](https://github.com/catchorg/Catch2/))
    ```shell script
    cd O-RPLY/
    git submodule update --init --recursive
    ```
3. Prepare build scripts and build the project!
    ```shell script
    cmake CMakeLists.txt
    make
    ```
   
## _O RPLY?_ in your CMake project

Let's imagine your project contains only one source file _main.cpp_ and _O RPLY?_ is in the _third-party/_ directory.
Now, all you have to do is this:

```CMake
cmake_minimum_required( VERSION 3.0 )

project( name-of-your-project VERSION 1.0 )

add_subdirectory( third-party/O-RPLY )

add_executable( your-app src/main.cpp)
target_link_libraries( your-app o-rply-lib ) # Include the O-RPLY? library
```

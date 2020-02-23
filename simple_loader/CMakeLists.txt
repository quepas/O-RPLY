cmake_minimum_required( VERSION 3.0 )

project( librply_simple_loader LANGUAGES CXX VERSION 1.0 )

add_library( librply_simple_loader STATIC src/PLYLoader.cpp )
set_property(TARGET librply_simple_loader PROPERTY POSITION_INDEPENDENT_CODE ON)
target_link_libraries( librply_simple_loader librply )
target_include_directories( librply_simple_loader PUBLIC include/)

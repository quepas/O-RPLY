cmake_minimum_required( VERSION 3.0 )

project( librply-tests LANGUAGES CXX VERSION 1.0 )

add_executable( librply-tests load_and_store_ply.cpp )
target_link_libraries( librply-tests librply librply_simple_loader Catch2::Catch2 )
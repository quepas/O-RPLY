cmake_minimum_required( VERSION 3.0 )

project( tests LANGUAGES CXX VERSION 1.0 )

set ( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR} )

add_executable( o-rply-tests load_and_store_ply.cpp )
target_link_libraries( o-rply-tests o-rply-lib o-rply-simple-loader Catch2::Catch2 )
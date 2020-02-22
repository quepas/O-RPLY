cmake_minimum_required( VERSION 3.0 )

project( librply VERSION 1.0 )

set ( CMAKE_CXX_STANDARD 17 )
set ( CMAKE_BUILD_TYPE Release )
set ( CMAKE_CXX_FLAGS_RELEASE -O3 )
set ( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/ )

add_library( librply STATIC src/rply.c )
set_property(TARGET librply PROPERTY POSITION_INDEPENDENT_CODE ON)
target_include_directories( librply PUBLIC include/)

add_executable( convert tools/convert.c )
target_link_libraries( convert librply )

add_executable( dump tools/dump.c )
target_link_libraries( dump librply )

add_executable( sconvert tools/sconvert.c )
target_link_libraries( sconvert librply )

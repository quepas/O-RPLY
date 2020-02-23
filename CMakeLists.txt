cmake_minimum_required( VERSION 3.0 )

project( librply VERSION 1.0 )

set ( CMAKE_CXX_STANDARD 17 )
set ( CMAKE_BUILD_TYPE Release )
set ( CMAKE_CXX_FLAGS_RELEASE -O3 )
set ( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/ )

set ( CATCH2_DIR third-party/Catch2 )
add_subdirectory ( ${CATCH2_DIR} )

add_library( librply STATIC src/rply.c )
set_property(TARGET librply PROPERTY POSITION_INDEPENDENT_CODE ON)
target_include_directories( librply PUBLIC include/)

add_subdirectory( simple_loader )
add_subdirectory( tests )
add_subdirectory( tools )
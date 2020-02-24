cmake_minimum_required( VERSION 3.0 )

project( O-RPLY? VERSION 1.0 )

set ( CMAKE_CXX_STANDARD 17 )
set ( CMAKE_BUILD_TYPE Release )
set ( CMAKE_CXX_FLAGS_RELEASE "-O3" )
set ( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/ )

set ( CATCH2_DIR third-party/Catch2 )
add_subdirectory ( ${CATCH2_DIR} )

add_subdirectory( lib )
add_subdirectory( simple_loader )
add_subdirectory( tests )
add_subdirectory( tools )
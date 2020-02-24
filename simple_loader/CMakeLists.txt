cmake_minimum_required( VERSION 3.0 )

project( simple-loader LANGUAGES CXX VERSION 1.0 )

add_library( o-rply-simple-loader STATIC src/PLYLoader.cpp )
set_property(TARGET o-rply-simple-loader PROPERTY POSITION_INDEPENDENT_CODE ON)
target_link_libraries( o-rply-simple-loader o-rply-lib )
target_include_directories( o-rply-simple-loader PUBLIC include/)

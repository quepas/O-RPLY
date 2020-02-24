cmake_minimum_required( VERSION 3.0 )

project( lib VERSION 1.0 )

add_library( o-rply-lib STATIC src/rply.c )
set_property(TARGET o-rply-lib PROPERTY POSITION_INDEPENDENT_CODE ON)
target_include_directories( o-rply-lib PUBLIC include/)

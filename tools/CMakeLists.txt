cmake_minimum_required( VERSION 3.0 )

project( tools VERSION 1.0 )

add_executable( convert convert.c )
target_link_libraries( convert librply )

add_executable( dump dump.c )
target_link_libraries( dump librply )

add_executable( sconvert sconvert.c )
target_link_libraries( sconvert librply )
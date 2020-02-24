cmake_minimum_required( VERSION 3.0 )

project( tools VERSION 1.0 )

add_executable( o-rply-convert convert.c )
target_link_libraries( o-rply-convert librply )

add_executable( o-rply-dump dump.c )
target_link_libraries( o-rply-dump librply )

add_executable( o-rply-sconvert sconvert.c )
target_link_libraries( o-rply-sconvert librply )
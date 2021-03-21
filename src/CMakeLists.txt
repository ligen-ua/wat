project(wat)
cmake_minimum_required(VERSION 2.8.2)

add_compile_options(-std=c++17)

if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()

add_subdirectory(whathappened_lib)
add_subdirectory(whathappened)

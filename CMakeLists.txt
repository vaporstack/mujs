cmake_minimum_required(VERSION 3.0)

project(mujs)

file(GLOB_RECURSE sources *.c src/*.c)
add_library(mujs STATIC ${sources})

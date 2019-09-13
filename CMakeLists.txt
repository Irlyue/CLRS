cmake_minimum_required(VERSION 3.13)
project(CLRS)

set(CMAKE_CXX_STANDARD 14)

include_directories(ch02)

add_executable(clrs main.cpp utils.h
        ch02/insertion_sort.h ch02/selection_sort.h ch02/merge_sort.h)

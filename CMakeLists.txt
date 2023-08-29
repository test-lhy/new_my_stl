cmake_minimum_required(VERSION 3.24)
project(my_stl)

set(CMAKE_CXX_STANDARD 14)

include_directories(my_std)

add_executable(my_stl
        my_std/vector.h my_std/basic.h vector_test.cpp)
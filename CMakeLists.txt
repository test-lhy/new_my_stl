cmake_minimum_required(VERSION 3.25)
project(my_stl)

set(CMAKE_CXX_STANDARD 14)

include_directories(my_std)

add_executable(my_stl
        my_std/vector.h vector_test.cpp my_std/sort.cpp my_std/random.cpp)
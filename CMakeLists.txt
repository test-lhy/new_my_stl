cmake_minimum_required(VERSION 3.24)
project(my_stl)

set(CMAKE_CXX_STANDARD 23)

include_directories(my_std)
aux_source_directory(my_std MYSTD)
add_executable(my_stl
        ${MYSTD}
        vector_improve_test.cpp
)
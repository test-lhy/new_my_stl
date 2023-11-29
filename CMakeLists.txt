cmake_minimum_required(VERSION 3.24)
project(my_stl)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(my_std)
aux_source_directory(my_std MYSTD)
add_executable(my_stl
        ${MYSTD}
        priority_queue_test.cpp
)
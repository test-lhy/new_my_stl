cmake_minimum_required(VERSION 3.24)
project(my_stl)

set(CMAKE_CXX_STANDARD 14)

include_directories(my_std)

add_executable(my_stl
        my_std/vector.h  my_std/sort.cpp my_std/random.cpp my_std/priority_queue.h my_std/basic.h my_std/algorithm.h my_std/mmath.h my_std/random.h my_std/sort.h my_std/list.h my_std/mmath.cpp my_std/algorithm.cpp vector_test.cpp)
cmake_minimum_required(VERSION 3.25)
project(my_stl)

set(CMAKE_CXX_STANDARD 14)

include_directories(my_std)

add_executable(my_stl
        my_std/vector.h  my_std/sort.cpp my_std/random.cpp my_std/priority_queue.h basic.h my_std/algorithm.h my_std/mmath.h vector_test.cpp my_std/random.h my_std/sort.h my_std/list.h)
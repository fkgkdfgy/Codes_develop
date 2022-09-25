project(HW2)
cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_FLAGS "-std=c++14")

include_directories("/usr/local/include/eigen3")
include_directories("/usr/include/eigen3")

add_executable(coordinate_calculate coordinate_calculate.cpp)
add_executable(matrix_calculation matrix_calculate.cpp)

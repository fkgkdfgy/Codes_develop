project(linear_list)
cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_FLAGS "-std=c++14")
set(CMAKE_BUILD_TYPE "Debug")

# include_directories(.)

add_executable(sequence_list_test sequence_list_test.cpp)

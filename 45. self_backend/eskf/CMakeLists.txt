project(new_code_eskf)

cmake_minimum_required( VERSION 3.5)

set(CMAKE_BUILD_TYPE Debug) 

set(CMAKE_CXX_FLAGS "-std=c++14 -o3")
include(../../cmake/eigen3.cmake)
include(../../cmake/sophus.cmake)
include(../../cmake/opencv.cmake)

add_executable(data_loader_test data_loader_test.cc)
add_executable(new_eskf_test new_eskf_test.cc)
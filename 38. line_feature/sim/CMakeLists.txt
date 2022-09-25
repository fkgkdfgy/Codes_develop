project(line_sim)
cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_FLAGS "-std=c++14")
set(CMAKE_BUILD_TYPE "Debug")

include(../../cmake/opencv.cmake)
include(../../cmake/pangolin.cmake)
include(../../cmake/eigen3.cmake)
include(../../cmake/ceres.cmake)

include_directories("/home/lwl/workspace/3rd-test-learning/38. line_feature/vanishing_point/code")

add_executable(test_sim test_sim.cc line_parameterization.cpp)
add_executable(test_angle test_angle.cc)
add_executable(test_case test_case.cc)
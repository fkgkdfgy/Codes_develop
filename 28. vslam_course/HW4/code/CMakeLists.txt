project(HW4)
cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_FLAGS "-std=c++14")
set(CMAKE_BUILD_TYPE "Debug")

include(../../../cmake/opencv.cmake)
include(../../../cmake/eigen3.cmake)
include(../../../cmake/pangolin.cmake)
add_executable(img_un image_undistort.cpp)
add_executable(disparity disparity.cpp)
add_executable(GN gaussnewton.cpp)
cmake_minimum_required(VERSION 3.5)
project(slam_demo)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS "-std=c++11")

include(../../cmake/pangolin.cmake)
include(../../cmake/opencv.cmake)
include(../../cmake/sophus.cmake)
include(../../cmake/eigen3.cmake)

set(src environment_builder.cpp
        common.cpp)

add_executable(main_test main_test.cpp ${src})

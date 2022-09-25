project(calibration)
cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_FLAGS "-std=c++14 ")

include(../../cmake/eigen3.cmake)
include(../../cmake/sophus.cmake)
include(../../cmake/opencv.cmake)




add_executable(extrinc_rot_calib extrinc_rot_calib.cpp )
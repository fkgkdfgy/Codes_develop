cmake_minimum_required(VERSION 3.5)
project(hw4_filter)
set(CMAKE_CXX_FLAGS "-std=c++14 -o3")
set(CMAKE_BUILD_TYPE "Debug")
include(../../cmake/opencv.cmake)
include(../../cmake/eigen3.cmake)
include(../../cmake/sophus.cmake)
include(../../cmake/ceres.cmake)
include(../../cmake/g2o.cmake)
include(../../cmake/omp.cmake)

include_directories(module)
include_directories(ekf)
list(APPEND SOURCE_EKF
    module/common.cpp
    module/ImuTypes.cc)

message(STATUS ${SOURCE_EKF})
add_executable(ekf_test ekf/ekf_test.cpp
                        ekf/kalman_filter.cpp
                        ${SOURCE_EKF}
                        )


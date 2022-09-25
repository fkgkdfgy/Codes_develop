project(hw3)
cmake_minimum_required(VERSION 3.5)
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS "-std=c++14 -o3")

include(../../cmake/ceres.cmake)
include(../../cmake/omp.cmake)
include(../../cmake/eigen3.cmake)
include(../../cmake/sophus.cmake)

# add_executable(calibra_accel hw_3_2_accel.cpp)
# add_executable(calibra_gyro hw_3_2_gyro.cpp)
# add_executable(semi_system_gyro hw_3_3.cpp)
# add_executable(pose_extrapolation hw_3_4.cpp)

add_executable(SO3_RK4_test SO3_RK4_test.cpp)

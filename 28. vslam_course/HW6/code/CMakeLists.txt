project(tracker)
cmake_minimum_required(VERSION 3.5)

# 不知道 openmp 没有加速的原因是不是因为这里 感觉应该不是

# add_definitions("-DENABLE_SSE")
set(CMAKE_CXX_FLAGS "-std=c++14 -o3")
set(CMAKE_BUILD_TYPE  "Release")

message("sse: " ${SSE_FLAGS})

include_directories(.)

include(../../../cmake/omp.cmake)
include(../../../cmake/opencv.cmake)
include(../../../cmake/eigen3.cmake)
include(../../../cmake/sophus.cmake)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_executable(test_common  test_common.cpp)
add_executable(test_case test_case.cpp)
add_executable(test_df test_double_float.cpp)
add_executable(test_dm test_case_dm.cpp)
add_executable(test_case_dm_zero_mean test_case_dm_zero_mean.cpp)
project(BAL_problem)
cmake_minimum_required(VERSION 3.5)
set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_FLAGS "-std=c++14 -o3")

include(${CMAKE_SOURCE_DIR}/../../../../cmake/g2o.cmake)
include(${CMAKE_SOURCE_DIR}/../../../../cmake/sophus.cmake)
include(${CMAKE_SOURCE_DIR}/../../../../cmake/eigen3.cmake)
include(${CMAKE_SOURCE_DIR}/../../../../cmake/PCL.cmake)

set(SRC
BAL_g2o.cpp
common.cpp)

add_executable(test_case_g2o_bal test_case_g2o_bal.cpp ${SRC})
add_executable(show_cloud show_cloud.cpp)


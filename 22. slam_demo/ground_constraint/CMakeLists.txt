project(ground_constraint)
set(CMAKE_CXX_FLAGS "-std=c++11")

set(CMAKE_BUILD_TYPE "Debug")

include(../../cmake/opencv.cmake)
include(../../cmake/eigen3.cmake)
include(../../cmake/sophus.cmake)
include(../../cmake/g2o.cmake)

add_executable(test_pool test_pool.cpp)
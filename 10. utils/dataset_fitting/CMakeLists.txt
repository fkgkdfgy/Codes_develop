project(ttt)
set(CMAKE_CXX_FLAGS "-std=c++11")
set(CMAKE_BUILD_TYPE "Debug")
include(../../cmake/eigen3.cmake)
add_executable(datasetft dataset_fitting_test.cpp)
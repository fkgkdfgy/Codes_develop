project(sophus_test)
cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_FLAGS "-std=c++11")



include_directories("/usr/local/include/eigen3")
include_directories("/usr/local/include/sophus")

add_executable(sophus_test sophus_test.cpp)
target_link_libraries(sophus_test ${SOPHUS_LIBRARIES})
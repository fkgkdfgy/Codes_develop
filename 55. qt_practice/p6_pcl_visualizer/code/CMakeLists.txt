project(test_pcl_viz)
cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_FLAGS "-std=c++11 -o3")

find_package(PCL REQUIRED)

include_directories(${PCL_INCLUDE_DIRS})

link_libraries(${PCL_LIBRARIES})
include_directories("/usr/local/include/eigen3")
include_directories("/usr/include/eigen3")

add_executable(test_pcl_viz main.cpp)
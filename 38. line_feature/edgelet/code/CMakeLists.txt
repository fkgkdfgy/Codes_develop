project(edgelet_tracker)
cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_FLAGS "-std=c++14 -o3")
set(CMAKE_BUILD_TYPE "Release")

find_package(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )
link_libraries(${OpenCV_LIBS})
message(${OpenCV_VERSION})

include(../../../cmake/eigen3.cmake)

add_executable(test_edgelet test_edgelet.cc)
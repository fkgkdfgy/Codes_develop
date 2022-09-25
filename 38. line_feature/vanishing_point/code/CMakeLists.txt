project(VP_test)
cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_FLAGS "-std=c++14")
set(CMAKE_BUILD_TYPE "Debug")

find_package(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )
link_libraries(${OpenCV_LIBS})

include_directories("/usr/local/include/eigen3")
include_directories("/usr/include/eigen3")

find_package(Ceres REQUIRED)
include_directories(${CERES_INCLUDE_DIRS})
link_libraries(${CERES_LIBRARIES})

add_executable(test_case test_case.cpp VPDetection.cpp)
# add_executable(extrinc_calib extrinc_calib.cpp)
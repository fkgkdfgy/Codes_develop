project(hw1_ceres_ndt)

cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_FLAGS "-std=c++11 ")
set(CMAKE_BUILD_TYPE DEBUG)
find_package(PCL REQUIRED)
find_package(Ceres REQUIRED)
find_package(glog REQUIRED)

include_directories(${PCL_INCLUDE_DIRS})
include_directories(.)
include_directories(${EIGEN_INCLUDE_DIRS})
include_directories("/usr/local/include/eigen3")
include_directories(${CERES_INCLUDE_DIRS})

add_executable(ceres_ndt ceres_ndt.cpp)
target_link_libraries(ceres_ndt
${PCL_LIBRARIES}
${CERES_LIBRARIES}
glog
)

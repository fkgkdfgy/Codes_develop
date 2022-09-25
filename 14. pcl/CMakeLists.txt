project(pcl_test)
cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_FLAGS "-std=c++11 -o3")
set(CMAKE_BUILD_TYPE DEBUG)
find_package(PCL REQUIRED)


include_directories(${PCL_INCLUDE_DIRS})
include_directories(.)
include_directories(${EIGEN_INCLUDE_DIRS})
include_directories("/usr/local/include/eigen3")

add_executable(icp_test registration/icp_test.cpp)
target_link_libraries(icp_test ${PCL_LIBRARIES})
add_executable(icp_display registration/icp_display.cpp)
target_link_libraries(icp_display ${PCL_LIBRARIES})



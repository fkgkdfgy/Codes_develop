project(kf_adjust)

cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_FLAGS "-std=c++11 -o3")
set(CMAKE_BUILD_TYPE "Release")

find_package(PCL REQUIRED)

include_directories(${PCL_INCLUDE_DIRS})

link_libraries(${PCL_LIBRARIES})
include_directories("/usr/local/include/eigen3")
include_directories("/usr/include/eigen3")

find_package(catkin REQUIRED COMPONENTS
  roscpp
  sensor_msgs
  rosbag
  nav_msgs
  pcl_conversions
  laser_geometry
)

include_directories(. ${catkin_INCLUDE_DIRS})
link_libraries(${catkin_LIBRARIES})


add_executable(test_rosbag_transfer test_rosbag_transfer.cpp)
add_executable(test_data_loader test_data_loader.cpp)
add_executable(test_kf_adjustor test_kf_adjustor.cpp)


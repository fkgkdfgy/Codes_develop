project(io_test)
cmake_minimum_required( VERSION 3.5)
set(CMAKE_CXX_FLAGS "-std=c++14 -o3")
set(CMAKE_BUILD_TYPE "Debug")

find_package(catkin REQUIRED COMPONENTS
  roscpp
  sensor_msgs
  rosbag
  cv_bridge
)


include_directories(. ${catkin_INCLUDE_DIRS})
link_libraries(${catkin_LIBRARIES})


add_executable(rosbag_convert "rosbag_convert.cpp")


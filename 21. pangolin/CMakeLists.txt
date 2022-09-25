project(pangolin_test)
cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_FLAGS "-std=c++11")

include(../cmake/sophus.cmake)
find_package(Pangolin REQUIRED)
include_directories(${Pangolin_INCLUDE_DIRS})

# add_executable(pangolin_cube pangolin_cube.cpp)
# target_link_libraries(pangolin_cube ${Pangolin_LIBRARIES})

# add_executable(pangolin_coordinate pangolin_coordinate.cpp)
# target_link_libraries(pangolin_coordinate ${Pangolin_LIBRARIES})

add_executable(pointcloud pointcloud.cpp)
target_link_libraries(pointcloud ${Pangolin_LIBRARIES})


add_executable(draw_trajectory draw_trajectory.cpp)
target_link_libraries(draw_trajectory ${Pangolin_LIBRARIES})
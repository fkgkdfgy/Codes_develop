project(jump_issue)
cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_FLAGS "-std=c++11 -o3")
set(CMAKE_BUILD_TYPE Debug)
find_package(PCL REQUIRED)

include_directories(${PCL_INCLUDE_DIRS})
include_directories(.)
include_directories(${EIGEN_INCLUDE_DIRS})
include_directories("/usr/local/include/eigen3")
link_libraries( ${PCL_LIBRARIES})
# boost filesystem
find_package(Boost COMPONENTS filesystem  REQUIRED)
if(NOT Boost_FOUND)
    message("Not found Boost")
endif()
include_directories(${Boost_INCLUDE_DIRS})
link_libraries(${Boost_LIBRARIES})

# YAML 
find_package(yaml-cpp REQUIRED)

include_directories(${YAML_CPP_INCLUDE_DIR})
link_libraries(yaml-cpp) 

add_executable(jump_analysis analysis.cc)
add_executable(test test_bad_scan.cc)
add_executable(new_voxel_filter test_new_voxel_filter.cc)
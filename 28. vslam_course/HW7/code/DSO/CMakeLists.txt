project(DSO)
cmake_minimum_required(VERSION 3.5)
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS "-std=c++14")


include(${CMAKE_SOURCE_DIR}/../../../../cmake/eigen3.cmake)
include(${CMAKE_SOURCE_DIR}/../../../../cmake/sophus.cmake)
include(${CMAKE_SOURCE_DIR}/../../../../cmake/opencv.cmake)
# include(${CMAKE_SOURCE_DIR}/../../../../cmake/pangolin.cmake)
include(${CMAKE_SOURCE_DIR}/../../../../cmake/g2o.cmake)


# find_package(Pangolin REQUIRED)

# include_directories(${Pangolin_INCLUDE_DIRS})

# link_libraries(${Pangolin_LIBRARIES})

add_executable(DSO directBA.cpp)
add_executable(sophus_eigen_test sophus_eigen_test.cc)


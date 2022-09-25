project(data_loader)
cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_FLAGS "-std=c++14")
set(CMAKE_BUILD_TYPE "Debug")


include(../../cmake/opencv.cmake)


# boost filesystem  
find_package(Boost COMPONENTS filesystem  REQUIRED)
if(NOT Boost_FOUND)
    message("Not found Boost")
endif()
include_directories(${Boost_INCLUDE_DIRS})
link_libraries(${Boost_LIBRARIES})

#opencv

FIND_PACKAGE(OpenCV REQUIRED)

INCLUDE_DIRECTORIES(
  ${OpenCV_INCLUDE_DIRS}
)

# Set link libraries
link_libraries(
  ${OpenCV_LIBS}
)

add_executable(data_loader data_loader_test.cpp)

add_executable(data_loader_func_test data_loader_func_test.cc)
project(tinyxml2_test)
cmake_minimum_required(VERSION 3.6)
set(CMAKE_BUILD_TYPE Debug)
set(SRC
src/tinyxml2.cpp
)
include_directories(.)

add_executable(tutorial_2 test/tutorial_2.cpp ${SRC})
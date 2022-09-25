project(random-generator)
cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_FLAGS "-std=c++11 -Wall")

add_executable(random_generator_test random_generator.cpp)
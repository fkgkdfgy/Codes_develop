project(slef_test)
cmake_minimum_required( VERSION 3.5)

add_definitions(-DSELF_TEST)
add_executable(add_definition add_definition.cpp)
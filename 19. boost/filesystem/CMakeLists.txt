project(jump_issue)
cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_FLAGS "-std=c++11 -o3")
set(CMAKE_BUILD_TYPE Debug)

# boost filesystem
find_package(Boost COMPONENTS filesystem REQUIRED)
if(NOT Boost_FOUND)
    message("Not found Boost")
endif()
include_directories(${Boost_INCLUDE_DIRS})
link_libraries(${Boost_LIBRARIES})


add_executable(filesystem_test filesystem_test.cpp)
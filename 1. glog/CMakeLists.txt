project(glog-test)
cmake_minimum_required(VERSION 3.5)
set(CMAKE_BUILD_TYPE DEBUG)
set(CMAKE_CXX_FLAGS "-std=c++11 -o3 -Wall")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./devel)

find_package(glog REQUIRED)

message(STATUS "Glog has been found " ${GLOG_FOUND})
message(STATUS glog)
message(STATUS ${GLOG_INCLUDE_DIRS})

# include_directories(${GLOG_INCLUDE_DIRS})

add_executable(glog-test ./src/glog-test.cpp)


## 找注意glog 的时候和别的库不一样 就是一个链接
target_link_libraries(glog-test
glog
)
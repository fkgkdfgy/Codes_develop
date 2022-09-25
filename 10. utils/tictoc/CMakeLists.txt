project(tic_toc)
cmake_minimum_required(VERSION 3.5)


## chrono 用C++11 开始加入标准库 所以使用需要添加 C++11 的flag
set(CMAKE_CXX_FlAGS "-std=c++11")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY devel)


add_executable(tic-toc-test tic_toc-test.cpp
                            tic_toc.cpp)
add_executable(tic_toc_duration tic_toc_duration.cpp)
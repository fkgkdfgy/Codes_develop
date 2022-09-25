project(data_collector)
set(CMAKE_CXX_FLAGS "-std=c++14")
include(../../cmake/eigen3.cmake)


add_executable(data_collect_test data_collect_test.cpp
                                 data_collector_T.cpp)
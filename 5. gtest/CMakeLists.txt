project(party-test)
cmake_minimum_required(VERSION 3.5)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY executable)

# 运行gtest 需要 C++11 标准

set(CMAKE_CXX_FLAGS "-std=c++11 -o3 -Wall")


## 5. gtest 一开始说是有4中方法


# find_package 的方法

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})


if(${GTEST_FOUND})
message(STATUS "==> GTEST_INCLUDE_DIR: " ${GTEST_INCLUDE_DIR})

# GTEST_BOTH_LIBRARIES 已经包括了 libgtest.a 和 libgtest_main.a 两个lib
message(STATUS "==> GTEST_LIBRARIES: " ${GTEST_LIBRARIES})
endif()

# gtest_1 测试 库的链接
add_executable(gtest-1 "gtest_1.cpp")
target_link_libraries(gtest-1 ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

# gtest_2 测试 gtest macro 的使用
add_executable(gtest-2 "gtest_2.cpp")
target_link_libraries(gtest-2 ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

# add_test 需要是一个地址 之前没有夹 executable 会报找不到 可执行文件
# add_test(gtest-1 executable/gtest-1)
# enable_testing()

# gtest_3 用于测试事件机制 和TEST_F
add_executable(gtest-3 "gtest_3.cpp")
target_link_libraries(gtest-3 ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

#gtest_test_f 用于更加系统的测试 TEST_F
add_executable(gtest-test-f "gtest_test_f.cpp")
target_link_libraries(gtest-test-f ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

add_test(gtest-3 executable/gtest-3)
enable_testing()
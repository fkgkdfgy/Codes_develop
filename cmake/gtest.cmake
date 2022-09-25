find_package(GTest REQUIRED)
find_package(Threads REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
link_libraries(${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
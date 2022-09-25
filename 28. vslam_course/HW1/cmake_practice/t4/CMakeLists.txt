project(t4)

# t4 主要任务是使用自己在t3创建的库

include_directories(../t3/share/include)
link_directories("/home/sunny/workspace_private/3rd-test-learning/16. cmake/cmake_practice/t3/lib")
link_libraries(libhello.a)
# link_libraries(libhello.so)
add_executable(t4 main.cpp)
project(cmake_practice)
cmake_minimum_required(VERSION 3.5)

message(STATUS "This is BINARY dir " ${PROJECT_BINARY_DIR})
message(STATUS "This is SOURCE dir "${PROJECT_SOURCE_DIR})

include_directories(include)
set(libhello_src src/hello.cpp)

# 设置库产生的位置
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
# 设置可执行文件产生的位置
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)



# 生成名称相同的动态库和静态库
add_library(hello SHARED ${libhello_src})
add_library(hello_static STATIC ${libhello_src})
# 这个语句用于设置输出的名称
SET_TARGET_PROPERTIES(hello_static PROPERTIES OUTPUT_NAME "hello")
# 动态库设置版本号 ?? VERSION 和 SOVERSION 的区别
SET_TARGET_PROPERTIES(hello PROPERTIES VERSION 1.2 SOVERSION 1)
link_libraries(hello)

# 使用下一级文件夹中的CMakeLists.txt 文件
# ADD_SUBDIRECTORY(source_dir [binary_dir] [EXCLUDE_FROM_ALL])
# binary_dir 代表中间结果和目标二进制文件所在处
# EXCLUDE_FROM_ALL 代表不进行编译的文件
add_subdirectory(src bin)


# 想要替代掉LD_LIBRARY_PATH 但是没有效果
# set(CMAKE_INSTALL_RPATH "/usr/local/lib")
# set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

install(TARGETS hello hello_static
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
install(DIRECTORY include
DESTINATION share)
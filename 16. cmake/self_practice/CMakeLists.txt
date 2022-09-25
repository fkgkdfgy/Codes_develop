# 这个CMakeLists.txt 文件主要是为了加深对于cmake 的使用
# 并且提供一个 之后可以直接复制使用的CMakeLists文件

cmake_minimum_required( VERSION 3.5)
project(cmake_test)

# 设置一个包括所有三方库的库
set(ALL_TARGET_LIBRARIES "")

# include cmake 文件进行第三方库的配置
# 内部使用 list 进行文件的添加和删除
# include(../cmake/PCL.cmake)
# include(../cmake/global_defination.cmake)
include(../cmake/opencv.cmake)

# 另外一种寻找库的方法
# FIND_LIBRARY(GLOG_LIB
#         NAMES glog glog.so libglog.so
#         PATH
#         /usr/lib/x86_64-linux-gnu
#         /opt/local/lib
#         /usr/lib
#         /usr/local/lib
#         )

# 给自身添加include路径
include_directories(include)

# GLOB_RECURSE 进行源文件搜索
file(GLOB_RECURSE ALL_SRCS "*.cpp")
file(GLOB_RECURSE NODE_SRCS "main.cpp")



# 使用list(REMOVE_ITEM )去除不需要的源文件
# list(REMOVE_ITEM ALL_SRCS ${NODE_SRCS})
# list(REMOVE_ITEM ALL_SRCS ${THIRD_PARTY_SRCS})

# 展示CMAKE 的内置路径
message(STATUS " 1. cmake_binary_dir: " ${CMAKE_BINARY_DIR})
message(STATUS " 2. cmake_source_dir: " ${CMAKE_SOURCE_DIR})
message(STATUS " 3. cmake_project_binary_dir: " ${PROJECT_BINARY_DIR})
message(STATUS " 4. cmake_project_source_dir: " ${PROJECT_SOURCE_DIR})

# 生成了一个SHARED库，add_library  + SHARED 之后就可以生成库了
# 如果没有添加 SHARED 或者 STATIC 就不会生成这个库
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(libtest ${OpenCV_LIBS})
add_library(libtest  SHARED
src/foo.cc)



add_executable(main_test src/main.cc)
target_link_libraries(main_test libtest)
set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY devel)

install(TARGETS main_test libtest 
        ARCHIVE DESTINATION devel/lib
        RUNTIME DESTINATION devel/bin
        LIBRARY DESTINATION devel/lib
)

install(DIRECTORY include/
  DESTINATION devel/include
  FILES_MATCHING PATTERN "*.h"
  PATTERN ".in" EXCLUDE
)

install(FILES
  include/global_defination.h.in
  DESTINATION devel/others
)
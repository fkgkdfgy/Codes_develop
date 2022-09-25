find_library(PCL REQUIRED)
include_directories(${PCL_INCLUDE_DIRS})
list(APPEND ALL_TARGET_LIBRARIES ${PCL_LIBRARIES})
# link_libraries 会给每一个add_executable 都链接上这个第三方库
# link_libraries(${PCL_LIBRARIES})
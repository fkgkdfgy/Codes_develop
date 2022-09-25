## 这个文件主要是为了满足 优化器的优化需要

list( APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/../../cmake )

message( status "cmake modudle path :" ${CMAKE_MODULE_PATH})
# 添加 cholmod 模块
find_package( Cholmod REQUIRED )
include_directories( ${CHOLMOD_INCLUDE_DIR} )

link_libraries( 
    ${CHOLMOD_LIBRARIES})
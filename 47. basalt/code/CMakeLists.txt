project(weird_pose_graph_method)

cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_FLAGS "-std=c++11")

include(../../cmake/eigen3.cmake)
include(../../cmake/g2o.cmake)
include(../../cmake/sophus.cmake)

# 这里需要绝对路径
list( APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/../../cmake )

message( status "cmake modudle path :" ${CMAKE_MODULE_PATH})
# 添加 cholmod 模块
find_package( Cholmod REQUIRED )
include_directories( ${CHOLMOD_INCLUDE_DIR} )

# add_executable(pose_graph_test pose_graph_test.cc)

# target_link_libraries( pose_graph_test
#     ${CHOLMOD_LIBRARIES})

# add_executable(sim_marg sim.cpp)

add_executable(assert_study assert_study.cc)
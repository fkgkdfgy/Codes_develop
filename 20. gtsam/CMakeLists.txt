cmake_minimum_required(VERSION 2.8.3)
project(gtsam_learning_and_test)

set(CMAKE_CXX_FLAGS "-std=c++11")

find_package(GTSAM REQUIRED QUIET)

include_directories(
    ${GTSAM_INCLUDE_DIR}
)

add_executable(exercise1_create_factor_graph 
               learning_example/exercise1_create_factor_graph.cpp)
target_link_libraries(exercise1_create_factor_graph gtsam)
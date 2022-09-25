project(g2o_test)
cmake_minimum_required( VERSION 3.5)
set(CMAKE_CXX_FLAGS "-std=c++14")
set(CMAKE_BUILD_TYPE "Debug")
find_package(OpenCV 3 REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

find_package(g2o REQUIRED)
include_directories(${G2O_INCLUDE_DIRS})

link_libraries( g2o_core g2o_stuff)


include (../cmake/eigen3.cmake)
include (../cmake/sophus.cmake)
# find_package(CSparse REQUIRED)
# find_package(Cholmod REQUIRED)
# include_directories(${CSPARSE_INCLUDE_DIR})
# include_directories(${EIGEN3_INCLUDE_DIR})
# include_directories(${CHOLMOD_INCLUDE_DIR})

# SET(G2O_LIBS g2o_cli g2o_ext_freeglut_minimal g2o_simulator 
# g2o_solver_slam2d_linear g2o_types_icp g2o_types_slam2d g2o_core 
# g2o_interface g2o_solver_csparse g2o_solver_structure_only 
# g2o_types_sba g2o_types_slam3d g2o_csparse_extension 
# g2o_opengl_helper g2o_solver_dense g2o_stuff g2o_types_sclam2d 
# g2o_parser g2o_solver_pcg g2o_types_data g2o_types_sim3 cxsparse )

include_directories(./g2o_basic_example)
include_directories(./g2o_slam_demo)

link_libraries(${OpenCV_LIBS})
add_executable(g2o_basic ./g2o_basic_example/g2o_curvefitting.cpp)

add_executable(g2o_powell_function ./g2o_basic_example/g2o_powell_function.cpp)

add_executable(g2o_icp_so3_r3 ./g2o_slam_demo/g2o_icp_so3_r3.cpp)
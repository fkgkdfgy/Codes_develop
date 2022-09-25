cmake_minimum_required(VERSION 2.8)

project(ceres-test)
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS "-std=c++11 -o3")


include(../cmake/ceres.cmake)
include(../cmake/g2o.cmake)
include(../cmake/PCL.cmake)
include(../cmake/eigen3.cmake)
include(../cmake/sophus.cmake)
include(../cmake/yaml.cmake)
# helloworld
# add_executable(ceres_icp_analystic_se3 ceres_self_example/ceres_icp_analystic_se3.cpp)
# target_link_libraries(ceres_icp_analystic_se3 ${CERES_LIBRARIES} )

# add_executable(ceres_icp_autodiff_q_r3 ceres_self_example/ceres_icp_autodiff_q_r3.cpp)
# target_link_libraries(ceres_icp_autodiff_q_r3 ${CERES_LIBRARIES} )

# add_executable(eigen_icp_so3_r3 ceres_self_example/eigen_icp_so3_r3.cpp)
# target_link_libraries(eigen_icp_so3_r3 ${CERES_LIBRARIES} )

add_executable(ceres_icp_analystic_so3_r3 ceres_self_example/ceres_icp_analystic_so3_r3.cpp)
target_link_libraries(ceres_icp_analystic_so3_r3 ${CERES_LIBRARIES} )

# add_executable(eigen_icp_so3_r3 ceres_self_example/eigen_svd_icp_test.cpp
#  ceres_self_example/eigen_svd_icp.cpp)
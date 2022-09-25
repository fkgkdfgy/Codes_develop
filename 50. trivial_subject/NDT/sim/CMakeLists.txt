project(ndt_sim)

set(CMAKE_CXX_FLAGS "-std=c++11 -o3")
set(CMAKE_BUILD_TYPE Debug)

include(../../../cmake/PCL.cmake)
include(../../../cmake/eigen.cmake)
include(../../../cmake/ceres.cmake)


project(eigen_test)


set(CMAKE_CXX_FLAGS "-std=c++11")
include_directories("/usr/local/include/eigen3")
include_directories("/usr/include/eigen3")
include_directories("/usr/local/include/sophus")

add_executable(eigen_permutation_matrix eigen_permutation_matrix.cpp)
add_executable(eigen_llt eigen_llt.cpp)

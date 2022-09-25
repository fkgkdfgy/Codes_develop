project(openmp_test)
set(CMAKE_CXX_FLAGS "-std=c++14")
############# 对于cmake 3.9 以下的版本只能使用这种方式
cmake_minimum_required(VERSION 3.5)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY devel)
if(NOT TARGET OpenMP::OpenMP_CXX)
    find_package(Threads REQUIRED)
    add_library(OpenMP::OpenMP_CXX IMPORTED INTERFACE)
    set_property(TARGET OpenMP::OpenMP_CXX
                 PROPERTY INTERFACE_COMPILE_OPTIONS ${OpenMP_CXX_FLAGS})
    # Only works if the same flag is passed to the linker; use CMake 3.9+ otherwise (Intel, AppleClang)
    set_property(TARGET OpenMP::OpenMP_CXX
                 PROPERTY INTERFACE_LINK_LIBRARIES ${OpenMP_CXX_FLAGS} Threads::Threads)
endif()

# gcc 编译标记 需要添加 -fopenmp
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
link_libraries(OpenMP::OpenMP_CXX)

# add_executable(hello_world omp_exercise1.cpp)
# add_executable(integratePi omp_exercise2.cpp)
# add_executable(critical_and_atomic omp_exercise3.cpp)
# add_executable(parallel_for omp_exercise4.cpp)
# add_executable(share_and_private omp_exercise4_private_shared.cpp)
# add_executable(barrier_nowait omp_exercise5.cpp)

add_executable(sections omp_exercise8_section.cpp)

############## 对于cmake 3.9 + 有了更好的处理方法

# cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
# find_package(OpenMP)
# if(OpenMP_CXX_FOUND)
#     target_link_libraries(MyTarget PUBLIC OpenMP::OpenMP_CXX)
# endif()
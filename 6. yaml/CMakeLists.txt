project(yaml-test)
cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_FLAGS "-std=c++11 -o3")

# find_package(yaml-cpp REQUIRED)

# include_directories(${YAML_CPP_INCLUDE_DIR})
# link_libraries( yaml-cpp)

find_package(Yaml REQUIRED)
target_include_directories(
    ${YAMLCPP_INCLUDE_DIR}

)
target_link_libraries(
    ${YAMLCPP_LIBRARIES}

)
add_executable(yaml-test yaml-test.cpp)
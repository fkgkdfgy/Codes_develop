project(data_loader)
cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_FLAGS "-std=c++14")
set(CMAKE_BUILD_TYPE "Debug")


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/example/_output)

include_directories(include/others)
include_directories(include/features)

set(OpenCV_DIR "/home/sunny/3rdparty/opencv3.3.1/share/OpenCV")

find_package(OpenCV 3.3 REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )
link_libraries(${OpenCV_LIBS})

message(STATUS "OpenCV library status:")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")


include(../cmake/eigen3.cmake)

set(SRC src/preprocess.cc
        src/tic_toc.cc
        src/line_extractor.cc)

# include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable(line_match example/test_line_match.cpp ${SRC}) 

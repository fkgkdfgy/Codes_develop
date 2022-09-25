find_package(catkin REQUIRED COMPONENTS
roscpp
tf
visualization_msgs
)

include_directories(
include
${catkin_INCLUDE_DIRS}
)
link_libraries(${catkin_LIBRARIES})
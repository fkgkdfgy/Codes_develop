cmake_minimum_required(VERSION 3.5)
project(test_ros2)

# Default to C99
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(std_msgs REQUIRED)
find_package(rclcpp REQUIRED)
find_package(behaviortree_cpp_v3 REQUIRED)
find_package(yaml_cpp_vendor REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(lifecycle_msgs REQUIRED)
find_package(rclcpp_lifecycle REQUIRED)
#find_package(rcl_interface REQUIRED)


set(dependencies
std_msgs
rclcpp
behaviortree_cpp_v3
yaml_cpp_vendor
geometry_msgs
lifecycle_msgs
rclcpp_lifecycle
#rcl_interface
)

# launch 测试Part
add_executable(test_param_node src/test_param_node.cpp)
target_include_directories(test_param_node PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
ament_target_dependencies(test_param_node ${dependencies})

add_executable(test_remap_pub_node src/test_remap_pub_node.cpp)
add_executable(test_remap_sub_node src/test_remap_sub_node.cpp)
add_executable(test_rosparam_node src/test_rosparam_node.cpp)

ament_target_dependencies(test_remap_pub_node ${dependencies})
ament_target_dependencies(test_remap_sub_node ${dependencies})
ament_target_dependencies(test_rosparam_node ${dependencies})

install(TARGETS test_param_node
  EXPORT export_${PROJECT_NAME}
  DESTINATION lib/${PROJECT_NAME})
install(TARGETS test_remap_pub_node
  EXPORT export_${PROJECT_NAME}
  DESTINATION lib/${PROJECT_NAME})
install(TARGETS test_remap_sub_node
  EXPORT export_${PROJECT_NAME}
  DESTINATION lib/${PROJECT_NAME})

install(TARGETS test_rosparam_node
  EXPORT export_${PROJECT_NAME}
  DESTINATION lib/${PROJECT_NAME})

# rqt 测试Part
#add_executable(test_rqt_dynamic_reconfigure_node src/test_rqt_dynamic_reconfigure_node.cpp)
#ament_target_dependencies(test_rqt_dynamic_reconfigure_node ${dependencies})

#install(TARGETS test_rqt_dynamic_reconfigure_node
#  EXPORT export_${PROJECT_NAME}
#  DESTINATION lib/${PROJECT_NAME})


# 同一个.cpp 两个不同名字的node 测试Part

add_executable(test_1to2_node src/test_1to2_node.cpp)
ament_target_dependencies(test_1to2_node ${dependencies})

install(TARGETS test_1to2_node
  EXPORT export_${PROJECT_NAME}
  DESTINATION lib/${PROJECT_NAME})


# yaml 读写测试

add_executable(test_yaml src/test_yaml_write.cpp)
ament_target_dependencies(test_yaml ${dependencies})

install(TARGETS test_yaml
  EXPORT export_${PROJECT_NAME}
  DESTINATION lib/${PROJECT_NAME})

# rviz 结合的yaml操作

add_executable(target_manager_node src/prototype_target_manager.cpp)
ament_target_dependencies(target_manager_node ${dependencies})

install(TARGETS target_manager_node
  EXPORT export_${PROJECT_NAME}
  DESTINATION lib/${PROJECT_NAME})

# rviz 显示目标点的操作

add_executable(target_printer src/test_rviz_target_print.cpp)
ament_target_dependencies(target_printer ${dependencies})

install(TARGETS target_printer
  EXPORT export_${PROJECT_NAME}
  DESTINATION lib/${PROJECT_NAME})








install(DIRECTORY param DESTINATION share/${PROJECT_NAME})
install(DIRECTORY launch DESTINATION share/${PROJECT_NAME})

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # uncomment the line when a copyright and license is not present in all source files
  #set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # uncomment the line when this package is not in a git repo
  #set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()

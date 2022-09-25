# demo_nodes 笔记
## 1. Cmake 语法细节

###  1.1  function() 命令
    
    
    function(\<name>[arg1 [arg2 [arg3 ...]]])

    COMMAND1(ARGS ...)

    COMMAND2(ARGS ...)

    endfunction(<name>)
    
    eg.

    function(custom_executable subfolder target)

    add_executable(${target} src/${subfolder}/${target}.cpp)
  ament_target_dependencies(${target}
    "example_interfaces"
    "rclcpp"
    "rcutils"
    "std_msgs")
  install(TARGETS ${target}
  DESTINATION lib/${PROJECT_NAME})

   endfunction()

## 2. 各个cpp分析
### 2.1 topic
#### 2.1.1 allocator_tutorial.cpp


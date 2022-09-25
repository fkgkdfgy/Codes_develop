/*
 * @Author: Liu Weilong
 * @Date: 2020-10-22 14:27:32
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2020-10-22 14:30:04
 * @FilePath: /3rd-test-learning/20. ros-foxy/src/test_ros2/src/bug_foxy_ros_ok.cpp
 * @Description: 偶然发现的ROS2神奇bug
 */

#include "rclcpp/rclcpp.hpp"

#include <iostream>

using namespace std;

int main(int argc, char ** argv)
{

    rclcpp::Node nh_;
    while(ros::ok())            // 使用了ros1 的ok 编译可以通过，但是会有一些不可预测的情况发生
    {
        rclcpp::spin(nh_.get_node_base_interface())
    }
}


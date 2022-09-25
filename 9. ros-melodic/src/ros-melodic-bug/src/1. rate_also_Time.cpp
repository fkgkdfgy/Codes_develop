/*
 * @Author: Liu Weilong
 * @Date: 2020-10-20 11:30:26
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2020-10-20 11:34:43
 * @FilePath: /3rd-test-learning/9. ros-melodic/src/ros-melodic-bug/src/1. rate_also_Time.cpp
 * @Description:  这个bug 主要是针对
 *                Cannot use ros::Time::now() before the first NodeHandle has been created or ros::start() has been called. 
 *                If this is a standalone app or test that just uses ros::Time and does not communicate over ROS, you may also call ros::Time::init() C++
 *                在之前的使用过程中，发现问题是 rate 之前必须要有一个ros::NodeHandle 存在
 * 
 * 
 */

#include <iostream>
#include "ros/ros.h"
using namespace std;

int main(int argc,char **argv)
{
    ros::Rate rate_(10); // 之前没有NodeHandle 的实例化 这里一定会报错
    rate_.sleep();
    return 0;
}

/*
 * @Author: Liu Weilong
 * @Date: 2020-09-25 08:56:51
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2020-09-28 16:05:53
 * @FilePath: /3rd-test-learning/9. ros-melodic/src/ros-melodic-test/src/9.5.1 multiple-spinner-test.cpp
 * @Description: 用于测试ros spin 的性质    
 */

#include <iostream>
#include <ros/ros.h>
#include <ros/timer.h>
#include "ros-melodic-test/test-use-function.hpp"
using namespace std;




int main(int argc,char **argv)
{
    // ros 不允许存在- . 等符号
    ros::init(argc,argv,"spinner_test");
    
    ros::NodeHandle nh;
    auto timer = nh.createTimer(ros::Duration(0.1),print);
    auto timer2 = nh.createTimer(ros::Duration(0.1),print1);
    // 单线程测试用例 是一个block spinner
    //ros::spin();
    // 多线程测试用例 会出现很明显的多线程运行情况
    ros::MultiThreadedSpinner spinners(4);
    spinners.spin(); // 也是一个block spinner 

    return 0;
}

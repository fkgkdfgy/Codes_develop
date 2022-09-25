/*
 * @Author: Liu Weilong
 * @Date: 2021-01-11 13:23:20
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-01-11 15:52:18
 * @FilePath: /3rd-test-learning/9. ros-melodic/src/ros-melodic-test/src/9.7 spin_and_ctrl_c.cpp
 * @Description: 
 * 
 *               spin() 在 control + C 之后会继续向下运行
 */
#include <iostream>
#include "ros/ros.h"

using namespace std;

int main(int argc, char** argv)
{
    ros::init(argc,argv,"aaa");

    
    ros::spin();
    
    cout<<"AAA"<<endl;
}
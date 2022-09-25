/*
 * @Author: Liu Weilong
 * @Date: 2020-09-28 15:51:23
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2020-09-28 15:57:34
 * @FilePath: /3rd-test-learning/9. ros-melodic/src/ros-melodic-test/include/ros-melodic-test/test-use-function.hpp
 * @Description: 
 */
#pragma once
#include "ros/ros.h"
#include <iostream>
using namespace std;
// spin callback 需要添加 ros::TimerEven& 作为参数
void print(const ros::TimerEvent& event){
    cout<<"===================================="<<endl;

    for (int i =0;i<30;i++)
    {
        cout<<i<<endl;
    }
}
void print1(const ros::TimerEvent& event){
    cout<<"===================================="<<endl;

    for (int i =30;i<60;i++)
    {
        cout<<i<<endl;
    }
}
void print_cbq(const ros::TimerEvent & event)
{
    cout<<"======= print_cbq 1111111111 ========"<<endl;
}

void print_cbq2(const ros::TimerEvent & event)
{
    cout<<"========= print cbq 22222222 ========="<<endl;
}
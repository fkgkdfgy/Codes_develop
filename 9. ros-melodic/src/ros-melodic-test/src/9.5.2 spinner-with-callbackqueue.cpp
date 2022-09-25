/*
 * @Author: Liu Weilong
 * @Date: 2020-09-28 15:47:14
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-01-06 17:08:03
 * @FilePath: /3rd-test-learning/9. ros-melodic/src/ros-melodic-test/src/9.5.2 spinner-with-callbackqueue.cpp
 * @Description:   用于测试CallBackQueue的使用 来让回调分NodeHandle 来进行 而不是都挤在一个回调队列内
 *                 主要内容： 1. 直接使用 callbackqueue 来进行回调函数处理
 *                          2. 使用spinner 来对callbackqueue 内的回调函数进行处理
 *                          3. 使用AynscSpinner     来对callbackqueue 内的回调函数进行处理    
 * 
 *                 MultiThreadedSpinner 和 AynscSpinner 的差别在于
 *                 AynscSpinner 需要手动开始        
 */

#include "ros/ros.h"
#include "ros/callback_queue.h"
#include "ros-melodic-test/test-use-function.hpp"


int main(int argc, char** argv)
{
    ros::init(argc,argv,"simple_spin_test");
    ros::NodeHandle nh1,nh2;
    ros::CallbackQueue cbq1,cbq2;
    nh1.setCallbackQueue(&cbq1);
    nh2.setCallbackQueue(&cbq2);

    ros::Timer t1 = nh1.createTimer(ros::Duration(0.1),print_cbq);
    ros::Timer t2 = nh2.createTimer(ros::Duration(0.1),print_cbq2);


    // 直接调用  CallBackQueue 来进行回调函数处理
    int i = 20;
    while(i>0)
    {
        // 这里的ros::WallDuration是一个等待时间
        cbq1.callAvailable(ros::WallDuration(0.1)); // 这里的 Duration 需要添加不然 默认为0 会找不到内容
        i--;
    }
    int j = 20;
    while(j>0)
    {
        cbq2.callAvailable(ros::WallDuration(0.1));
        j--;
    }

    // MultiThreadedSpinner 来对回调函数进行处理

    cout<<"============ using spin to handle callback()============="<<endl;
    ros::MultiThreadedSpinner spinner(4);
    spinner.spin(&cbq2);


    // // 使用AsyncSpinner 来对回调函数进行处理
    // ros::AsyncSpinner asyapinner(4,&cbq1);
    // asyapinner.start(); // 开始进行回调

    // ros::waitForShutdown(); //不加上的话就只会 spin 一次




    return 0;
}
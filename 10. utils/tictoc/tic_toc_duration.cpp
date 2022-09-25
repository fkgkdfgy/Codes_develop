/*
 * @Author: Liu Weilong
 * @Date: 2020-10-22 10:23:01
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2020-10-22 10:36:43
 * @FilePath: /3rd-test-learning/11. tic toc/tic_toc_duration.cpp
 * @Description: 主要是用于测试duration 的使用
 *               原因： 1. 在对于ros2 里面的WallTimer nh.create_wall_timer 的 std::chrono::duration 的时间段的把握有问题
 *               
 * 
 *               经过测试 std::chrono::duration<double> tmp(int(x)) x 代表秒
 */
#include <iostream>
#include <chrono>
using namespace std;

int main()
{

    
    std::chrono::duration<double> tmp(300);
    cout<<" the corresponding nanosecond is " 
        << std::chrono::duration_cast<std::chrono::nanoseconds>(tmp).count()<<endl;

    cout<<" the corresponding microsecond is " 
        << std::chrono::duration_cast<std::chrono::microseconds>(tmp).count()<<endl;

    cout<<" the corresponding millisecond is " 
        << std::chrono::duration_cast<std::chrono::milliseconds>(tmp).count()<<endl;

    return 0;
}
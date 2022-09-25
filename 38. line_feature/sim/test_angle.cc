/*
 * @Author: Liu Weilong
 * @Date: 2021-03-10 17:55:56
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-03-10 18:08:39
 * @FilePath: /3rd-test-learning/38. line_feature/sim/test_angle.cc
 * @Description: 
 */
#include <iostream>
#include <vector>

#include "sophus/so2.hpp"

using namespace std;

int main()
{
    Sophus::SO2d angle1 = Sophus::SO2d::exp(-0.1);
    Sophus::SO2d angle2 = Sophus::SO2d::exp(-0.3);

    cout<< (angle1*angle2).log()<<endl;
    cout<< (angle2*angle1).log()<<endl;
}
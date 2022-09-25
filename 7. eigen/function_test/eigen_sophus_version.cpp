/*
 * @Author: Liu Weilong
 * @Date: 2021-01-20 08:05:56
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-01-20 08:20:53
 * @Description: 
 */

#include <iostream>

#include "Eigen/Eigen"
#include "sophus/so3.hpp"

using namespace std;

int main()
{
    Eigen::Vector3d temp(3.0,4.0,5.0);
    
    Eigen::Matrix3d skew = Sophus::SO3d::hat(temp);
}
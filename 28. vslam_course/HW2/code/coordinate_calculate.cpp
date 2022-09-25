/*
 * @Author: Liu Weilong
 * @Date: 2021-01-24 20:07:40
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-01-24 20:18:45
 * @Description: 
 */

#include <iostream>
#include "Eigen/Eigen"

using namespace std;

int main()
{
    Eigen::Quaterniond q1(0.55,0.3,0.2,0.2);
    Eigen::Vector3d t1(0.7,1.1,0.2);
    Eigen::Quaterniond q2(-0.1,0.3,-0.7,0.2);
    Eigen::Vector3d t2(-0.1,0.4,0.8);
    
    q1.normalize();
    q2.normalize();
    Eigen::Matrix4d Tc1w = Eigen::Matrix4d::Identity();
    Tc1w.block<3,3>(0,0) = q1.matrix();
    Tc1w.block<3,1>(0,3) = t1;
    Eigen::Matrix4d Tc2w = Eigen::Matrix4d::Identity();
    Tc2w.block<3,3>(0,0) = q2.matrix();
    Tc2w.block<3,1>(0,3) = t2;
    Eigen::Matrix4d Tc2c1 = Tc2w * Tc1w.inverse();
    
    Eigen::Vector3d p1(0.5,-0.1,0.2);
    
    auto p2 = Tc2c1.block<3,3>(0,0)*p1+ Tc2c1.block<3,1>(0,3);

    cout<<"the result is "<<endl<<p2.transpose()<<endl;
}
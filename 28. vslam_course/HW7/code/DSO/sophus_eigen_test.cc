/*
 * @Author: Liu Weilong
 * @Date: 2021-03-14 11:29:09
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-03-14 11:34:25
 * @Description: 
 */
#include <iostream>
#include <sophus/se3.hpp>
#include <sophus/so3.hpp>
#include <Eigen/Eigen>



int main()
{
    Eigen::Matrix<double,6,1> temp;
    temp.Ones();
    Sophus::SE3d se3 = Sophus::SE3d::exp(temp);
    std::cout<<se3.matrix3x4()<<std::endl;
}
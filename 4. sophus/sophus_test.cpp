/*
 * @Author: Liu Weilong 
 * @Date: 2020-09-02 10:03:08 
 * @Last Modified by: Liu Weilong
 * @Last Modified time: 2020-09-02 10:39:41
 */

#include <iostream>

#include "Eigen/Eigen"
#include "sophus/geometry.hpp"
#include "sophus/so3.hpp"
#include "sophus/se3.hpp"

using namespace Eigen;
using namespace Sophus;
using namespace std;

int main()
{
    float initAngle = 0.3;

    // 从 Vector3f 生成 旋转矩阵 Matrix3f
    // 此处的重点是SO3 后要显式地 实例化<foat >
    auto initRot = SO3<float>::exp(Vector3f(0,0,initAngle)).matrix();
    cout<<initRot<<endl;


    
    // 结合Eigen::Map的使用
    
    double t[6]={0,0,0,0,0,0};
    Eigen::Map<Eigen::Matrix<double,6,1>> map_(t);

    Sophus::SE3d se3_ = Sophus::SE3d::exp(map_);

    cout<<" the se3 is "<<endl
    << se3_.matrix()<<endl;

    
    // Eigen::Map + Sophus 结合使用
    // 此处可以知道 0,0,0,1 前四位对应 四元数 最后一位是实数位
    // 后三位是 位移位

    double se3_array [] = {0,0,0,1,10,10,10};
    Eigen::Map<Sophus::SE3d> se3_map(se3_array);
    
    cout<<"the SE3 matrix is "<<endl
        << se3_map.matrix3x4()<<endl; 
    
    cout<<"the log of SE3 is "<<endl
        << se3_map.log()      <<endl;

    return 0;
}
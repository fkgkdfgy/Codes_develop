/*
 * @Author: Liu Weilong
 * @Date: 2021-01-21 17:41:03
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-02-07 16:42:40
 * @FilePath: /3rd-test-learning/33. slam_test/include/features/plucker.h
 * @Description: 
 */
#include <iostream>

#include "Eigen/Eigen"


using namespace std;

bool GeneratePlane(Eigen::Vector3d point_1,
                   Eigen::Vector3d point_2,
                   Eigen::Vector3d point_3,
                   Eigen::Vector4d & plane_params)
{
    Eigen::Vector3d vec_12 = point_2-point_1;
    Eigen::Vector3d vec_13 = point_3-point_1;

    Eigen::Vector3d n = vec_12.cross(vec_13);

    if(n.norm()<1e-7)
    {return false;}

    double w = n.dot(point_1);
    // 实验了一下
    plane_params<<n,-w;
    return true;
}

bool GeneratorPlucker(Eigen::Vector4d plane_1,
                      Eigen::Vector4d plane_2,
                      Eigen::Matrix<double,6,1> & plucker)
{
    Eigen::Matrix4d L = plane_1*plane_2.transpose() -plane_2*plane_1.transpose();   
    Eigen::Matrix3d dx = L.block<3,3>(0,0);
    Eigen::Vector3d n = L.block<3,1>(0,3);
    Eigen::Vector3d d (-dx(1,2),dx(0,2),-dx(0,1));
    
    plucker<<d,n;
    
    return true;
}

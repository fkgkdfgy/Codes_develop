/*
 * @Author: Liu Weilong
 * @Date: 2021-01-17 10:37:07
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-01-21 13:50:13
 * @Description: 
 */

#pragma once
#include <iostream>
#include <vector>

#include "Eigen/Eigen"
#include "opencv2/core/core.hpp"

#define _S_SLAM_DEMO_ namespace slam_demo {
#define _E_SLAM_DEMO_ };
_S_SLAM_DEMO_


//  基础方法类
class FrameInterface
{
    public:
    using UVs = std::vector<Eigen::Vector2d> ;
    using H = Eigen::Matrix3d;
    // 四点生成H 矩阵
    // x_2 = Hx_1
    static bool EpipolarH4Pts(const UVs & uvs_1,const UVs & uvs_2,H& H12);
    // x_2^Tt^Rx_1 = 0;
    static bool EpipolarF8Pts(const UVs & uvs_1,const UVs & uvs_2,H& E12);

    static bool MotionRecovery(const H& E12, Eigen::Matrix4d & transform);
};

bool FrameInterface::EpipolarH4Pts(const UVs & uvs_1,const UVs & uvs_2,H& H12)
{
    if(uvs_1.size()!=uvs_2.size()||uvs_1.size()<4)
    return false;
    
    Eigen::MatrixXd A(uvs_1.size()*2,9); 
    A.setZero();
    Eigen::Matrix<double,2,9> temp_A;
    
    for(int i =0; i<4;i++)
    {
        auto & x1 = uvs_1.at(i);
        auto & x2 = uvs_2.at(i);
        temp_A<<x1.x(),x1.y(),1,0,0,0,-1*x2.x()*x1.x(),-1*x2.x()*x1.y(),-1*x2.x(),
                0,0,0,x1.x(),x1.y(),1,-1*x2.y()*x1.x(),-1*x2.y()*x1.y(),-1*x2.y();
        A.block<2,9>(i*2,0) = temp_A;
    }

    Eigen::SelfAdjointEigenSolver<Eigen::Matrix<double,9,9>> eigen_solver(A.transpose()*A);
    
    eigen_solver.eigenvalues().minCoeff();
    auto & eigen_value = eigen_solver.eigenvalues();
    auto temp = std::min_element(eigen_value.data(),eigen_value.data()+eigen_value.size());
    Eigen::Matrix<double,9,1> h = eigen_solver.eigenvectors().col(temp-eigen_value.data());
    
    Eigen::Matrix<double,3,3,Eigen::RowMajor> H;
    memcpy(H.data(),h.data(),9*8);
    double w = H.row(2).dot((Eigen::Vector3d()<<uvs_1.front().x(),uvs_1.front().y(),1.0).finished());
    H12 = H/w;
    if(h.norm()>1e8)
    return false;
}
// x_2^Tt^Rx_1 = 0;
bool FrameInterface::EpipolarF8Pts(const UVs & uvs_1,const UVs & uvs_2,H& F12)
{
    if(uvs_1.size()!=uvs_2.size()||uvs_1.size()<8)
    return false;
    // 构造 kronecker product
    Eigen::Matrix<double,1,9> one_row;
    Eigen::MatrixXd A(uvs_2.size(),9);
    for(int i = 0;i<8;i++)
    {
        auto & x2 = uvs_2[i];
        Eigen::Vector3d uv_3d = (Eigen::Vector3d()<<uvs_1[i],1.0).finished();
        one_row<< x2.x()*uv_3d.transpose(),
                  x2.y()*uv_3d.transpose(),
                  1.0*uv_3d.transpose();
        A.block<1,9>(i,0)<<one_row;
    }
    cout<<" the A is "<<endl<<A<<endl;
    Eigen::SelfAdjointEigenSolver<Eigen::Matrix<double,9,9>> eigen_solver(A.transpose()*A);
    
    eigen_solver.eigenvalues().minCoeff();
    auto & eigen_value = eigen_solver.eigenvalues();

    auto temp = std::min_element(eigen_value.data(),eigen_value.data()+eigen_value.size());
    Eigen::Matrix<double,9,1> h = eigen_solver.eigenvectors().col(temp-eigen_value.data());
    cout<<"A*h"<<endl<<A*h<<endl;
    cout<<"h::"<<endl<<h<<endl;
    Eigen::Matrix<double,3,3,Eigen::RowMajor> F;
    memcpy(F.data(),h.data(),9*8);
    double w = F.row(2).dot((Eigen::Vector3d()<<uvs_1.front().x(),uvs_1.front().y(),1.0).finished());
    F12 = F/w;
}


_E_SLAM_DEMO_

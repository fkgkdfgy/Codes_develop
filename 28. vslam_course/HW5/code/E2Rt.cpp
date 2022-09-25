/*
 * @Author: Liu Weilong
 * @Date: 2021-02-21 11:59:46
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-02-21 22:14:26
 * @Description: 
 */
//
// Created by 高翔 on 2017/12/19.
// 本程序演示如何从Essential矩阵计算R,t
//

#include "Eigen/Eigen"

using namespace Eigen;

#include "sophus/so3.hpp"

#include <iostream>

using namespace std;

int main(int argc, char **argv) {

    // 给定Essential矩阵
    Matrix3d E;
    E << -0.0203618550523477, -0.4007110038118445, -0.03324074249824097,
            0.3939270778216369, -0.03506401846698079, 0.5857110303721015,
            -0.006788487241438284, -0.5815434272915686, -0.01438258684486258;

    // 待计算的R,t
    Matrix3d R;
    Vector3d t;

    // SVD and fix sigular values
    // START YOUR CODE HERE

    Eigen::JacobiSVD<Eigen::Matrix3d> jsvd(E,ComputeFullU | ComputeFullV);
    jsvd.compute(E);
    Eigen::Matrix3d U = jsvd.matrixU();
    Eigen::Matrix3d V = jsvd.matrixV();
    Eigen::Vector3d temp_E = jsvd.singularValues();
    // 调整E
    Eigen::Vector3d T(temp_E.x()/2+temp_E.y()/2,
                      temp_E.x()/2+temp_E.y()/2,
                      0.0);

    Eigen::AngleAxisd aa1(M_PI_2,Eigen::Vector3d(0,0,1));
    Eigen::AngleAxisd aa2(-1*M_PI_2,Eigen::Vector3d(0,0,1));

    // END YOUR CODE HERE

    // set t1, t2, R1, R2 
    // START YOUR CODE HERE
    Matrix3d t_wedge1;
    Matrix3d t_wedge2;

    Matrix3d R1;
    Matrix3d R2;

    Eigen::Matrix3d T_d;
    T_d<<T.x(),0.0,0.0,T.y(),0.0,0.0,0.0,0.0,0.0;

    t_wedge1 = U*aa1.matrix()*T_d*U.transpose();
    t_wedge2 = U*aa2.matrix()*T_d*U.transpose();

    R1 = U*aa1.matrix().transpose()*V.transpose();
    R2 = U*aa2.matrix().transpose()*V.transpose();

    // END YOUR CODE HERE

    cout << "R1 = " << R1 << endl;
    cout << "R2 = " << R2 << endl;
    cout << "t1 = " << Sophus::SO3d::vee(t_wedge1) << endl;
    cout << "t2 = " << Sophus::SO3d::vee(t_wedge2) << endl;

    // check t^R=E up to scale
    Matrix3d tR = t_wedge1 * R1;
    cout << "t^R = " << tR << endl;

    return 0;
}
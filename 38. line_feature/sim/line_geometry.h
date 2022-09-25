/*
 * @Author: Liu Weilong
 * @Date: 2021-03-11 09:46:25
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-03-15 09:55:33
 * @FilePath: /3rd-test-learning/38. line_feature/sim/line_geometry.h
 * @Description: 
 */

#pragma once

#include <iostream>
#include "Eigen/Eigen"
#include "sophus/se3.hpp"

using namespace std;
using namespace Eigen;

typedef Matrix<double,6,1> Vector6d;
typedef Matrix<double,8,1> Vector8d;
typedef Matrix<double,6,6> Matrix6d;

// pl-vio

Vector4d pi_from_ppp(Vector3d x1, Vector3d x2, Vector3d x3) {
    Vector4d pi;
    pi << ( x1 - x3 ).cross( x2 - x3 ), - x3.dot( x1.cross( x2 ) ); // d = - x3.dot( (x1-x3).cross( x2-x3 ) ) = - x3.dot( x1.cross( x2 ) )
    return pi;
}

// 两平面相交得到直线的plucker 坐标
Vector6d pipi_plk( Vector4d pi1, Vector4d pi2){
    Vector6d plk;
    Matrix4d dp = pi1 * pi2.transpose() - pi2 * pi1.transpose();

    plk << dp(0,3), dp(1,3), dp(2,3), - dp(1,2), dp(0,2), - dp(0,1);
    return plk;
}

Matrix6d T_of_line(const Sophus::SO3d & so3, const Eigen::Vector3d & translation)
{
    Matrix6d t_of_line;
    t_of_line.setZero();
    t_of_line.block<3,3>(0,0) = so3.matrix();
    t_of_line.block<3,3>(0,3) = Sophus::SO3d::hat(translation)*so3.matrix();
    t_of_line.block<3,3>(3,3) = so3.matrix();
    return t_of_line;
}

Vector4d plk_to_orth(Vector6d plk)
{
    Vector4d orth;
    Vector3d n = plk.head(3);
    Vector3d v = plk.tail(3);

    Vector3d u1 = n/n.norm();
    Vector3d u2 = v/v.norm();
    Vector3d u3 = u1.cross(u2);

    // todo:: use SO3
    orth[0] = atan2( u2(2),u3(2) );
    orth[1] = asin( -u1(2) );
    orth[2] = atan2( u1(1),u1(0) );

    Vector2d w( n.norm(), v.norm() );
    w = w/w.norm();
    orth[3] = asin( w(1) );

    return orth;
}

Vector6d orth_to_plk(Vector4d orth)
{
    Vector6d plk;

    Vector3d theta = orth.head(3);
    double phi = orth[3];

    double s1 = sin(theta[0]);
    double c1 = cos(theta[0]);
    double s2 = sin(theta[1]);
    double c2 = cos(theta[1]);
    double s3 = sin(theta[2]);
    double c3 = cos(theta[2]);

    Matrix3d R;
    R <<
      c2 * c3,   s1 * s2 * c3 - c1 * s3,   c1 * s2 * c3 + s1 * s3,
            c2 * s3,   s1 * s2 * s3 + c1 * c3,   c1 * s2 * s3 - s1 * c3,
            -s2,                  s1 * c2,                  c1 * c2;

    double w1 = cos(phi);
    double w2 = sin(phi);
    double d = w1/w2;      // 原点到直线的距离

    Vector3d u1 = R.col(0);
    Vector3d u2 = R.col(1);

    Vector3d n = w1 * u1;
    Vector3d v = w2 * u2;

    plk.head(3) = n;
    plk.tail(3) = v;

    //Vector3d Q = -R.col(2) * d;
    //plk.head(3) = Q.cross(v);
    //plk.tail(3) = v;

    return plk;


}

Eigen::Vector3d to_extend(const Eigen::Vector2d & p)
{
    return (Eigen::Vector3d()<<p,1.0).finished();
}

Vector4d to_pl_vio(const Eigen::Vector2d & s, const Eigen::Vector2d & e,const Camera & cm)
{
    Eigen::Vector4d obs1 ; obs1<<cm.p2c(s).head<2>(),cm.p2c(e).head<2>();
    return obs1;
}


Vector4d to_pl_vio(const Eigen::Vector3d & s, const Eigen::Vector3d & e)
{
    Eigen::Vector4d obs1 ; obs1<<s.head<2>(),e.head<2>();
    return obs1;
}

// pl-slam
Eigen::Vector3d direction(const Eigen::Vector2d & s, const Eigen::Vector2d &e)
{
        Vector3d sp_l; sp_l << s.x(), s.y(), 1.0;
        Vector3d ep_l; ep_l << e.x(), e.y(), 1.0;
        Vector3d le_l; le_l << sp_l.cross(ep_l); le_l = le_l / std::sqrt( le_l(0)*le_l(0) + le_l(1)*le_l(1) );
        return le_l;
}
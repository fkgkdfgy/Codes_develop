/*
 * @Author: Liu Weilong
 * @Date: 2021-03-10 09:20:27
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-03-15 09:52:20
 * @FilePath: /3rd-test-learning/38. line_feature/sim/sim_env.h
 * @Description: 
 */
#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "opencv2/core/core.hpp"
#include "opencv2/opencv.hpp"
#include "Eigen/Eigen"
#include "sophus/se3.hpp"

using namespace std;

class Line 
{
    public:
    Eigen::Vector3d start_point;
    Eigen::Vector3d end_point;
};

class LinePx
{
    public:
    Eigen::Vector2d start_px;
    Eigen::Vector2d end_px;
    cv::Point2f cv_start_px;
    cv::Point2f cv_end_px;
    Eigen::Vector3d ld;
    void Update();
};

void LinePx::Update()
{
    cv_start_px.x = start_px.x();
    cv_start_px.y = start_px.y();
    cv_end_px.x = end_px.x();
    cv_end_px.y = end_px.y();
}

class Camera
{
    public:
    
    using M23 = Eigen::Matrix<double,2,3>;
    using M66 = Eigen::Matrix<double,6,6>;
    using M61 = Eigen::Matrix<double,6,1>;
    static double fx,fy,cx,cy;
    static double width,height;

    Sophus::SE3d T_w_c;
    Eigen::Vector2d c2p(Eigen::Vector3d point) const ;
    Eigen::Vector3d p2c(Eigen::Vector2d px,double depth=1.0)const;  
    Eigen::Vector3d c2w(Eigen::Vector3d point) const;
    Eigen::Vector3d w2c(Eigen::Vector3d point) const;
    M61 w2cl(M61 line) const;
    M23 J_uv_xyz(Eigen::Vector3d point)const;
    // La = T_a_b L_b
    M66 J_lc_lw()const;
};


double Camera::fx = 307.3070580996126;
double Camera::fy = 307.3070580996126;
double Camera::cx = 372.865217584225;
double Camera::cy = 240.0338623810578;
double Camera::width = 752;
double Camera::height = 480;


Eigen::Vector2d Camera::c2p(Eigen::Vector3d point) const
{
    Eigen::Vector2d px;
    point /= point.z();
    px << fx*point.x() + cx,
          fy*point.y() + cy;
    return px;
}

Eigen::Vector3d Camera::p2c(Eigen::Vector2d px,double depth) const
{
    Eigen::Vector3d point;
    point<< (px.x()-cx)/fx,
            (px.y()-cy)/fy,
            1.0;
    point *= depth;
    return point;
}

Eigen::Vector3d Camera::c2w(Eigen::Vector3d point_c) const
{
    Eigen::Vector3d p_w = T_w_c * point_c;
    return p_w;
}

Eigen::Vector3d Camera::w2c(Eigen::Vector3d point_w) const
{
    Eigen::Vector3d p_c = T_w_c.inverse() * point_w;
    return p_c;
}

Camera::M23 Camera::J_uv_xyz(Eigen::Vector3d point) const
{
    M23 result;
    double z_inv = 1./point.z();
    double z_inv_2 = z_inv*z_inv;

    result<< fx*z_inv,0.,-1.*fx*point.x()*z_inv_2,
             0.,fy*z_inv,-1.*fy*point.y()*z_inv_2;

    return result;
}

Camera::M66 Camera::J_lc_lw()const
{
    Sophus::SE3d T_c_w = T_w_c.inverse();
    M66 Tau_c_w;
    Tau_c_w.setZero();
    Tau_c_w.block<3,3>(0,0) = T_c_w.so3().matrix();
    Tau_c_w.block<3,3>(0,3) = Sophus::SO3d::hat(T_c_w.translation())*T_c_w.so3().matrix();
    Tau_c_w.block<3,3>(3,3) = T_c_w.so3().matrix();

    return Tau_c_w;
}

Camera::M61 Camera::w2cl(Camera::M61 line_w) const
{
    Sophus::SE3d T_c_w = T_w_c.inverse();
    M66 Tau_c_w;
    Tau_c_w.setZero();
    Tau_c_w.block<3,3>(0,0) = T_c_w.so3().matrix();
    Tau_c_w.block<3,3>(0,3) = Sophus::SO3d::hat(T_c_w.translation())*T_c_w.so3().matrix();
    Tau_c_w.block<3,3>(3,3) = T_c_w.so3().matrix();

    return Tau_c_w*line_w;
}
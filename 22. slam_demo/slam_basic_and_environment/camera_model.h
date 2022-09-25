/*
 * @Author: Liu Weilong
 * @Date: 2021-01-17 11:34:40
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-01-20 07:44:37
 * @Description: 
 */

#pragma once

#include <iostream>
#include <vector>
#include <string>

#include "opencv2/core/core.hpp"
#include "Eigen/Eigen"

#define _S_SLAM_DEMO_ namespace slam_demo {
#define _E_SLAM_DEMO_ };

using namespace std;

_S_SLAM_DEMO_

class PinholeCameraParams
{
    public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    PinholeCameraParams(const string & config_path);
    const Eigen::Matrix3d & GetInstrincMatrix()const {return instrinc_matrix_;}
    double fx_,fy_,cx_,cy_;
    int width_,height_;  
    Eigen::Matrix3d instrinc_matrix_;
    
};

class CameraModelPinhole
{
    public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    CameraModelPinhole(const string & config_path);

    bool CameraPointsToUVs(const std::vector<Eigen::Vector3d> & points_camera,
                           std::vector<Eigen::Vector2d> & uvs);
    bool UVToCameraPoint(const std::vector<Eigen::Vector2d> & uvs,
                         std::vector<Eigen::Vector3d> & points_camera);
                           

    PinholeCameraParams camera_params_;
};

PinholeCameraParams::PinholeCameraParams(const string & path)
{
    cv::FileStorage fin(path,CV_STORAGE_READ);
    if(!fin.isOpened())
    {
        cerr<<"[ERROR]: there is something wrong with the path"<<endl;
        abort();
    }
    
    fin["Camera.fx"]>>fx_;
    fin["Camera.fy"]>>fy_;
    fin["Camera.cx"]>>cx_;
    fin["Camera.cy"]>>cy_;
    fin["Camera.width"]>>width_;
    fin["Camera.height"]>>height_;

    cout<<"Camera.fx : "<<fx_<<endl;
    cout<<"Camera.fy : "<<fy_<<endl;
    cout<<"Camera.cx : "<<cx_<<endl;
    cout<<"Camera.cy : "<<cy_<<endl;
    cout<<"Camera.width : "<<width_<<endl;
    cout<<"Camera.height : "<<height_<<endl;

    instrinc_matrix_<<fx_,0,cx_,
                      0,fy_,cy_,
                      0,0,1;
    cout<<"instrinc matrix : "<< endl<<instrinc_matrix_<<endl;
}

CameraModelPinhole::CameraModelPinhole(const string & config_path):camera_params_(config_path){}

bool CameraModelPinhole::CameraPointsToUVs(const std::vector<Eigen::Vector3d> & points_camera,
                                           std::vector<Eigen::Vector2d> & uvs)
{
    double fx,fy,cx,cy;
    auto camera_intrinsic = camera_params_.GetInstrincMatrix();
    uvs.clear();
    uvs.reserve(points_camera.size());
    fx = camera_intrinsic(0,0);
    fy = camera_intrinsic(1,1);
    cx = camera_intrinsic(0,2);
    cy = camera_intrinsic(1,2);

    for(auto & point_camera:points_camera)
    {
        Eigen::Vector2d uv;
        auto temp = point_camera/point_camera.z();
        uv<<(temp.x()*fx+cx),
            (temp.y()*fy+cy);
        uvs.emplace_back(uv);
    }
    return true;
}


bool CameraModelPinhole::UVToCameraPoint(const std::vector<Eigen::Vector2d> & uvs,
                                         std::vector<Eigen::Vector3d> & points_camera)
{
    double fx,fy,cx,cy;
    auto camera_intrinsic = camera_params_.GetInstrincMatrix();
    fx = camera_intrinsic(0,0);
    fy = camera_intrinsic(1,1);
    cx = camera_intrinsic(0,2);
    cy = camera_intrinsic(1,2);

    points_camera.clear();
    points_camera.reserve(uvs.size());
    
    for(auto & uv:uvs)
        points_camera.emplace_back((uv.x()-cx)/fx,(uv.y()-cy)/fy,1.0);

    return true;
}


_E_SLAM_DEMO_
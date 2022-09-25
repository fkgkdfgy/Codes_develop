/*
 * @Author: Liu Weilong
 * @Date: 2021-01-05 09:33:25
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-01-20 07:44:13
 * @FilePath: /3rd-test-learning/25. slam_demo/environment_builder.h
 * @Description: 
 */
#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include <string>

#include "Eigen/Eigen"
#include "opencv2/core/core.hpp"

using namespace std;


#define _S_SLAM_DEMO_ namespace slam_demo {
#define _E_SLAM_DEMO_ };


_S_SLAM_DEMO_

using Point = Eigen::Vector3d;
using Transfrom = Eigen::Matrix4d;
using Rot = Eigen::Matrix3d;
using InsParam = Eigen::Matrix3d;
using Translation = Eigen::Vector3d;
using UV = Eigen::Vector2d;


class Landmark
{
    public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    Eigen::Vector3d position_;
    unsigned int label_;
    bool view_ = false;
};
class EnvironmentBuilderOptions
{
    public:
    EnvironmentBuilderOptions(const string & config_path);
    double x_min_, y_min_,z_min_;
    double x_scale_,y_scale_,z_scale_;
    double x_interval_,y_interval_,z_interval_;
    bool initial = false;
};
class EnvironmentBuilder
{
    public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    EnvironmentBuilder(const string config_path);
    bool BuildEnvironment(double x_min,double y_min,double z_min ,
                          double x_scale, double y_scale, double z_scale ,
                          double x_interval,double y_interval,double z_interval,
                          std::vector<Landmark,Eigen::aligned_allocator<Landmark>> &landmark_array);
    
    std::vector<Landmark,Eigen::aligned_allocator<Landmark>> & GetLandmarks(){return landmarks_;}
    std::vector<Eigen::Vector3d> & GetLandmarksVec3d(){return landmarks_v3d_;}

    private:
    const string config_path_;
    EnvironmentBuilderOptions options_;
    std::vector<Landmark,Eigen::aligned_allocator<Landmark>>  landmarks_;
    std::vector<Eigen::Vector3d> landmarks_v3d_;
};

template<class Allocator>
bool LandmarkToVec3d(const std::vector<Landmark,Allocator> & landmarks ,
                     std::vector<Eigen::Vector3d> & landmarks_vec3d)
{
    landmarks_vec3d.clear();
    landmarks_vec3d.reserve(landmarks.size());
    for(auto & ld:landmarks)
    {
        landmarks_vec3d.push_back(ld.position_);
    }
}



_E_SLAM_DEMO_
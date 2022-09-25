/*
 * @Author: Liu Weilong
 * @Date: 2020-11-11 09:27:28
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-01-05 09:50:26
 * @FilePath: /3rd-test-learning/25. slam_demo/build_test_environment.h
 * @Description: 用于建立测试环境 拥有数据一致性
 */

#include "Eigen/Eigen"
#include <iostream>
#include <vector>
#include <cmath>


void transfromPointIntoNewFrame(const Eigen::Vector3d &  point_old,Eigen::Vector3d & point_new,
                                const Eigen::Matrix3d & rotation,const Eigen::Vector3d& translation)
{
    point_new = rotation * point_old + translation;
}

void transformNormalizedPointIntoUV(const Eigen::Matrix3d & intrinc_params,const Eigen::Vector3d landmark,
                                    Eigen::Vector3d & uv)
{
    uv = intrinc_params * landmark/landmark.z();
}

bool buildEnvironment(double x_min,double y_min,double z_min ,
                      double x_scale, double y_scale, double z_scale ,
                      double x_interval,double y_interval,double z_interval,
                      std::vector<Landmark> &landmark_array)
{
    if(x_scale<0||y_scale<0 || z_scale<0)
    {
        std::cout<<" the scale should be positive"<<std::endl;
        return false;
    }
    unsigned int count =0;
    landmark_array.clear();
    landmark_array.reserve(static_cast<size_t>(std::ceil(x_scale/x_interval))*
                           static_cast<size_t>(std::ceil(y_scale/y_interval))*
                           static_cast<size_t>(std::ceil(z_scale/z_interval)));
    for(double x_cur = 0.0;x_cur<x_scale;x_cur+=x_interval)
    {
        for(double y_cur = 0.0;y_cur<y_scale;y_cur+=y_interval)
        {
            for(double z_cur=0.0;z_cur<z_scale;z_cur+=z_interval)
            {
                Landmark tmp_lk;
                tmp_lk.position_ = Eigen::Vector3d(x_cur+x_min,y_cur+y_min,z_cur+z_min);
                tmp_lk.label_ = count;
                landmark_array.push_back(tmp_lk);
                count++;
            }
        }
    }

    return true;
}

// 这里的fov 是半视场角 而不是全视场 R_c_w t_c_w
bool findPointInCamera(double fov_width,double fov_height,const std::vector<Landmark>& landmark_array, 
                        const Eigen::Matrix3d &rotation, const Eigen::Vector3d & translation,
                        std::vector<Landmark> * landmark_in_camera,std::vector<unsigned int> & index,
                        bool transformed = true)
{   
    if(fov_width<0 || fov_height<0)
    {
        std::cout<<"the fov should be positive"<<std::endl;
        return false;
    }
    if(landmark_array.size()==0)
    {
        std::cout<<"[WARNING]: the landmark_array is invalid"<<std::endl;
        return false;
    } 
    
    auto rotation_inv = rotation.transpose();
    auto translation_inv = -1* rotation_inv*translation;
    std::vector<Landmark> landmark_in_camera_frame;
    landmark_in_camera_frame.resize(landmark_array.size(),Landmark());
    for(int i=0;i<landmark_array.size();i++)
    {
        transfromPointIntoNewFrame(landmark_array[i].position_,landmark_in_camera_frame[i].position_,rotation_inv,translation_inv);
        landmark_in_camera_frame[i].label_ = landmark_array[i].label_;
        const auto & l = landmark_in_camera_frame[i];

        double z_angle = std::atan2(l.position_.y(),l.position_.x());
        double y_angle = std::atan2(l.position_.z(),l.position_.x());
        // if(fabs(z_angle)>M_PI_2)
        // {
        //     z_angle = M_PI-z_angle;
        // }
        // if(y_angle>M_PI_2)
        // {
        //     y_angle = M_PI-y_angle;
        // }
        if(fabs(z_angle)<fov_width && fabs(y_angle)<fov_height)
        {
            if(transformed== true)
            {
            landmark_in_camera->push_back(landmark_in_camera_frame[i]);
            }
            else{
            landmark_in_camera->push_back(landmark_array[i]);
            }
        }
        index.push_back(i);
    }
    return true;
}




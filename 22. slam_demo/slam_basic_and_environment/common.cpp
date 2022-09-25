/*
 * @Author: Liu Weilong
 * @Date: 2021-01-18 06:40:59
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-01-18 06:48:21
 * @Description: 
 */
#include "common.h"
_S_SLAM_DEMO_

bool TransformPoints(const std::vector<Eigen::Vector3d> & points_world,
                     const Eigen::Matrix4d & Twc,
                     std::vector<Eigen::Vector3d> & points_camera)
{
    points_camera.clear();
    points_camera.reserve(points_world.size());
    const Eigen::Matrix3d R = Twc.block<3,3>(0,0);
    const Eigen::Vector3d t = Twc.block<3,1>(0,3);
    for(auto & point_world:points_world)
        points_camera.push_back(R*point_world+t);
    return true;
}


bool NormalizePoints(const std::vector<Eigen::Vector3d> & points_camera,
                     std::vector<Eigen::Vector3d> & normalized_points)
{
    normalized_points.clear();
    normalized_points.reserve(points_camera.size());
    for(auto & point:points_camera)
        normalized_points.emplace_back(point/point.z());
    return true;
}

_E_SLAM_DEMO_
/*
 * @Author: Liu Weilong
 * @Date: 2021-01-18 06:40:53
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-01-18 07:35:00
 * @Description: 
 */

#pragma once

#include <vector>

#include "Eigen/Eigen"
#include "opencv2/core/core.hpp"

#define _S_SLAM_DEMO_ namespace slam_demo {
#define _E_SLAM_DEMO_ };


_S_SLAM_DEMO_

bool TransformPoints(const std::vector<Eigen::Vector3d> & points_world,
                     const Eigen::Matrix4d & Twc,
                     std::vector<Eigen::Vector3d> & points_camera);

bool NormalizePoints(const std::vector<Eigen::Vector3d> & points_camera,
                     std::vector<Eigen::Vector3d> & normalized_points);





_E_SLAM_DEMO_
/*
 * @Author: Liu Weilong
 * @Date: 2020-12-03 07:37:47
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2020-12-13 20:47:04
 * @Description: 预积分
 * 
 *               这里使用的方式 是 Forster 14。
 *               这里是ORB3的具体实现
 * 
 * 
 */

#ifndef IMUTYPES_H
#define IMUTYPES_H

#include<vector>
#include<utility>
#include<opencv2/core/core.hpp>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <Eigen/Dense>
#include <mutex>
#include "common.h"


// Lie Algebra Functions
cv::Mat ExpSO3(const float &x, const float &y, const float &z);
Eigen::Matrix<double,3,3> ExpSO3(const double &x, const double &y, const double &z);
cv::Mat ExpSO3(const cv::Mat &v);
cv::Mat LogSO3(const cv::Mat &R);
cv::Mat RightJacobianSO3(const float &x, const float &y, const float &z);
cv::Mat RightJacobianSO3(const cv::Mat &v);
cv::Mat InverseRightJacobianSO3(const float &x, const float &y, const float &z);
cv::Mat InverseRightJacobianSO3(const cv::Mat &v);
cv::Mat Skew(const cv::Mat &v);
cv::Mat NormalizeRotation(const cv::Mat &R);
Eigen::MatrixXd TypeTransform(const cv::Mat & m);

#endif // IMUTYPES_H

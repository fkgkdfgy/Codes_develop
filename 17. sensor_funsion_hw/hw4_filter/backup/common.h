/*
 * @Author: Liu Weilong
 * @Date: 2020-11-22 16:04:51
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2020-12-04 22:11:31
 * @Description: 
 */
#pragma once

#include <g2o/stuff/sampler.h>
#include "sophus/so3.hpp"
#include "opencv2/core/core.hpp"
#include <iostream>

bool AddNoise(std::vector<Eigen::Vector3d> & point_cloud);

bool AddNoiseSO3(std::vector<Eigen::Vector3d> & point_cloud);

void prepareLaserSimData(const std::vector<Eigen::Vector3d> & real_rotation_data,
                         const std::vector<Eigen::Vector3d> & real_translation_data,
                         std::vector<Eigen::Vector3d> & noise_rotation_data,
                         std::vector<Eigen::Vector3d> & noise_translation_data,
                         double IMU_hz , double measure_hz);

void showMat(cv::Mat & mat);


template<typename T,int rows>
void transformIntoVector(const std::vector<std::vector<double>> & data,
                         std::vector<Eigen::Matrix<T,rows,1>> & output)
{
    using type = Eigen::Matrix<T,rows,1>; 
    output.clear();
    output.reserve(data.front().size());
    for(int i=0;i<data.front().size();i++)
    {
        int N = data.size();
        type element;
        for(int j=0;j<N;j++)
        {
            element(j) = data.at(j).at(i);
        }
        output.push_back(element);
    }
}

void transformIntoVector1(const std::vector<std::vector<double>> & data,
                         std::vector<double> & output);

Eigen::MatrixXd TypeTransform(const cv::Mat & m);

// 这里固定返回的值是 CV_32F 的数值类型
template<typename T,int rows,int cols>
cv::Mat TypeTransformInv(const Eigen::Matrix<T,rows,cols> & matrix)
{
    cv::Mat matrix_result = cv::Mat::zeros(matrix.rows(),matrix.cols(),CV_32F);
    for(int i =0;i<matrix.rows();i++)
    for(int j =0;j<matrix.cols();j++)
    matrix_result.at<float>(i,j)= matrix(i,j);


    return matrix_result;
}


/*
 * @Author: Liu Weilong
 * @Date: 2021-01-18 07:33:28
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-01-21 10:46:27
 * @Description: 
 */

#pragma once

#include <vector>

#include "Eigen/Eigen"
#include "opencv2/core/core.hpp"

#define _S_SLAM_DEMO_ namespace slam_demo {
#define _E_SLAM_DEMO_ };

_S_SLAM_DEMO_


class Converter
{
    public:
    
    static std::vector<cv::Point2f> toPoints(const std::vector<Eigen::Vector2d> & uvs);
    static std::vector<Eigen::Vector2d> toVector2ds(const std::vector<cv::Point2f> & uvs);
    static Eigen::Matrix3d toEigenM33d(const cv::Mat & mat);


    template<int N, int M>
    static cv::Mat toMat(const Eigen::Matrix<double,N,M> & temp);
    template<typename Allocator,int N, int M>
    static std::vector<cv::Mat> toMats(const std::vector<Eigen::Matrix<double,N,M>,Allocator> & temp);
    
    
};

Eigen::Matrix3d Converter::toEigenM33d(const cv::Mat & mat)
{
    Eigen::Matrix3d M33;
    M33<<mat.at<double>(0,0),mat.at<double>(0,1),mat.at<double>(0,2),
         mat.at<double>(1,0),mat.at<double>(1,1),mat.at<double>(1,2),
         mat.at<double>(2,0),mat.at<double>(2,1),mat.at<double>(2,2);
    return M33;
}


std::vector<cv::Point2f> Converter::toPoints(const std::vector<Eigen::Vector2d> & uvs)
{
    std::vector<cv::Point2f> pts;
    pts.reserve(uvs.size());
    
    for(auto & uv:uvs)
    {
        pts.push_back(cv::Point2f(uv.x(),uv.y()));
    }
    return pts;
}

std::vector<Eigen::Vector2d> Converter::toVector2ds(const std::vector<cv::Point2f> & uvs)
{
    std::vector<Eigen::Vector2d> pts;
    pts.reserve(uvs.size());
    
    for(auto & uv:uvs)
    {
        pts.push_back(Eigen::Vector2d(uv.x,uv.y));
    }
    return pts;
}

template<int N, int M>
cv::Mat Converter::toMat(const Eigen::Matrix<double,N,M> & temp)
{
    cv::Mat temp_cv(N,M,CV_64F);
    for(int row = 0;row<temp_cv.rows;row++)
    {
        double * row_ptr = temp_cv.ptr<double>(row);
        for(int col=0;col<temp_cv.cols;col++)
        {
            row_ptr[col] = temp(row,col);
        }
    }
    return temp_cv;
}

template<typename Allocator,int N, int M>
std::vector<cv::Mat> Converter::toMats(const std::vector<Eigen::Matrix<double,N,M>,Allocator> & eigen_matrix_array)
{
    std::vector<cv::Mat> cv_matrix_array;
    cv_matrix_array.reserve(eigen_matrix_array.size());

    for(const auto & eigen_matrix:eigen_matrix_array)
    {
        cv::Mat temp = toMat(eigen_matrix);
        cv_matrix_array.push_back(temp);
    }

    return cv_matrix_array;
}
_E_SLAM_DEMO_
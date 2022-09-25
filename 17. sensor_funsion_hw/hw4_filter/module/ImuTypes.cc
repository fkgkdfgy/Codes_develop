/**
* This file is part of ORB-SLAM3
*
* Copyright (C) 2017-2020 Carlos Campos, Richard Elvira, Juan J. Gómez Rodríguez, José M.M. Montiel and Juan D. Tardós, University of Zaragoza.
* Copyright (C) 2014-2016 Raúl Mur-Artal, José M.M. Montiel and Juan D. Tardós, University of Zaragoza.
*
* ORB-SLAM3 is free software: you can redistribute it and/or modify it under the terms of the GNU General Public
* License as published by the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* ORB-SLAM3 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
* the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with ORB-SLAM3.
* If not, see <http://www.gnu.org/licenses/>.
*/

#include "ImuTypes.h"
#include<iostream>


const float eps = 1e-4;

cv::Mat NormalizeRotation(const cv::Mat &R)
{
    cv::Mat U,w,Vt;
    cv::SVDecomp(R,w,U,Vt,cv::SVD::FULL_UV);
    // assert(cv::determinant(U*Vt)>0);
    return U*Vt;
}

cv::Mat Skew(const cv::Mat &v)
{
    const float x = v.at<float>(0);
    const float y = v.at<float>(1);
    const float z = v.at<float>(2);
    return (cv::Mat_<float>(3,3) << 0, -z, y,
            z, 0, -x,
            -y,  x, 0);
}

cv::Mat ExpSO3(const float &x, const float &y, const float &z)
{
    cv::Mat I = cv::Mat::eye(3,3,CV_32F);
    const float d2 = x*x+y*y+z*z;
    const float d = sqrt(d2);
    cv::Mat W = (cv::Mat_<float>(3,3) << 0, -z, y,
                 z, 0, -x,
                 -y,  x, 0);
    if(d<eps)
        return (I + W + 0.5f*W*W);
    else
        return (I + W*sin(d)/d + W*W*(1.0f-cos(d))/d2);
}

Eigen::Matrix<double,3,3> ExpSO3(const double &x, const double &y, const double &z)
{
    Eigen::Matrix<double,3,3> I = Eigen::MatrixXd::Identity(3,3);
    const double d2 = x*x+y*y+z*z;
    const double d = sqrt(d2);
    Eigen::Matrix<double,3,3> W;
    W(0,0) = 0;
    W(0,1) = -z;
    W(0,2) = y;
    W(1,0) = z;
    W(1,1) = 0;
    W(1,2) = -x;
    W(2,0) = -y;
    W(2,1) = x;
    W(2,2) = 0;

    if(d<eps)
        return (I + W + 0.5*W*W);
    else
        return (I + W*sin(d)/d + W*W*(1.0-cos(d))/d2);
}

cv::Mat ExpSO3(const cv::Mat &v)
{
    return ExpSO3(v.at<float>(0),v.at<float>(1),v.at<float>(2));
}

cv::Mat LogSO3(const cv::Mat &R)
{
    const float tr = R.at<float>(0,0)+R.at<float>(1,1)+R.at<float>(2,2);
    cv::Mat w = (cv::Mat_<float>(3,1) <<(R.at<float>(2,1)-R.at<float>(1,2))/2,
                                        (R.at<float>(0,2)-R.at<float>(2,0))/2,
                                        (R.at<float>(1,0)-R.at<float>(0,1))/2);
    const float costheta = (tr-1.0f)*0.5f;
    if(costheta>1 || costheta<-1)
        return w;
    const float theta = acos(costheta);
    const float s = sin(theta);
    if(fabs(s)<eps)
        return w;
    else
        return theta*w/s;
}

cv::Mat RightJacobianSO3(const float &x, const float &y, const float &z)
{
    cv::Mat I = cv::Mat::eye(3,3,CV_32F);
    const float d2 = x*x+y*y+z*z;
    const float d = sqrt(d2);
    cv::Mat W = (cv::Mat_<float>(3,3) << 0, -z, y,
                 z, 0, -x,
                 -y,  x, 0);
    if(d<eps)
    {
        return cv::Mat::eye(3,3,CV_32F);
    }
    else
    {
        return I - W*(1.0f-cos(d))/d2 + W*W*(d-sin(d))/(d2*d);
    }
}

cv::Mat RightJacobianSO3(const cv::Mat &v)
{
    return RightJacobianSO3(v.at<float>(0),v.at<float>(1),v.at<float>(2));
}

cv::Mat InverseRightJacobianSO3(const float &x, const float &y, const float &z)
{
    cv::Mat I = cv::Mat::eye(3,3,CV_32F);
    const float d2 = x*x+y*y+z*z;
    const float d = sqrt(d2);
    cv::Mat W = (cv::Mat_<float>(3,3) << 0, -z, y,
                 z, 0, -x,
                 -y,  x, 0);
    if(d<eps)
    {
        return cv::Mat::eye(3,3,CV_32F);
    }
    else
    {
        return I + W/2 + W*W*(1.0f/d2 - (1.0f+cos(d))/(2.0f*d*sin(d)));
    }
}

cv::Mat InverseRightJacobianSO3(const cv::Mat &v)
{
    return InverseRightJacobianSO3(v.at<float>(0),v.at<float>(1),v.at<float>(2));
}

Eigen::MatrixXd TypeTransform(const cv::Mat & m)
{    
    Eigen::MatrixXd m_eigen(m.rows,m.cols);
    for(int i =0;i<m.rows;i++)
    {
        for(int j =0;j<m.cols;j++)
        {
            m_eigen(i,j) = m.at<float>(i,j);
        }
    }
    return m_eigen;
}

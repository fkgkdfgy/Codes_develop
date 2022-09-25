/*
 * @Author: Liu Weilong
 * @Date: 2021-01-24 14:34:22
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-02-01 16:29:33
 * @FilePath: /3rd-test-learning/33. vertical_line_slam/src/preprocess.cc
 * @Description: 
 */
#include "preprocess.h"

MyneyePreprocessor::MyneyePreprocessor(const string & path)
{
    LoadParams(path);
}
void MyneyePreprocessor::LoadParams(const string & config)
{
    cv::Mat R, T, M2, D2, M1, D1;
    cv::FileStorage fsread(config.c_str(), CV_STORAGE_READ);
    cv::Mat R1, R2, P1, P2, Q, rmap[2][2];
    if (fsread.isOpened())
    {
        fsread["M1"] >> M1;
        fsread["D1"] >> D1;
        fsread["M2"] >> M2;
        fsread["D2"] >> D2;
        fsread["R"] >> R;
        fsread["T"] >> T;
        fsread["Camera.fx"]>>fx;
        fsread["Camera.fy"]>>fy;
        fsread["Camera.cx"]>>cx;
        fsread["Camera.cy"]>>cy;
    }
    
    float balance = fsread["Rectify.balance"];
    float fov_scale = fsread["Rectify.fov_scale"];
    std::cout<<"=============================================="<<std::endl;
    std::cout<<balance<<std::endl;
    std::cout<<fov_scale<<std::endl;
    fsread.release();

    cv::fisheye::stereoRectify(M1, D1, M2, D2, cv::Size(752,480), R, T, R1, R2, P1, P2,
        Q, CV_CALIB_ZERO_DISPARITY, cv::Size(752,480), balance, fov_scale);
    cv::fisheye::initUndistortRectifyMap(M1, D1, R1, P1, cv::Size(752,480), CV_32FC1, M1l,M2l);
    cv::fisheye::initUndistortRectifyMap(M2, D2, R2, P2, cv::Size(752,480), CV_32FC1, M1r,M2r);  
    
}

bool MyneyePreprocessor::UnDistort(const cv::Mat & img_l_raw, const cv::Mat & img_r_raw,
                         cv::Mat & img_l,cv::Mat & img_r)
{
    cv::remap(img_l_raw,img_l,M1l,M2l,cv::INTER_LINEAR);
    cv::remap(img_r_raw,img_r,M1r,M2r,cv::INTER_LINEAR);
    return true;
}
Eigen::Vector4d MyneyePreprocessor::GetInstrincParam()const
{
    return Eigen::Vector4d(fx,fy,cx,cy);
}
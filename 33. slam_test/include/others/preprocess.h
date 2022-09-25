/*
 * @Author: Liu Weilong
 * @Date: 2021-01-24 10:48:48
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-02-01 16:28:58
 * @FilePath: /3rd-test-learning/33. vertical_line_slam/include/preprocess.h
 * @Description: 
 */

#include <string>
#include <vector>
#include <iostream>

#include "opencv2/core/core.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "Eigen/Eigen"


using namespace std;

class MyneyePreprocessor
{
    public:
    MyneyePreprocessor(const string & config);
    bool UnDistort(const cv::Mat & img_l_raw, const cv::Mat & img_r_raw,
                   cv::Mat & img_l,cv::Mat & img_r);
    bool HistogramMatching(const cv::Mat & img_pre,cv::Mat & img_cur) = delete;
    Eigen::Vector4d GetInstrincParam()const;

    private:
    void LoadParams(const string & config);

    private:
    cv::Mat M1l,M2l,M1r,M2r;
    double fx,fy,cx,cy;
};


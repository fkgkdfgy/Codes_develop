/*
 * @Author: Liu Weilong
 * @Date: 2021-02-27 23:30:44
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-03-06 18:29:36
 * @Description: 
 */

#pragma once

#include <iostream>
#include <vector>
#include <string>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "eigen3/Eigen/Eigen"

using namespace std;

template<typename Allocator>
void cv2eigen(const vector<cv::Point2f> & input, 
              vector<Eigen::Vector2f,Allocator> & output)
{
    output.clear();
    for(auto & p:input)
        output.push_back(Eigen::Vector2f(p.x,p.y));
}




template<typename Allocator>
void eigen2cv(const vector<Eigen::Vector2f,Allocator> & input,
              vector<cv::Point2f> & output)
{
    output.clear();
    for(auto & p:input)
        output.push_back(cv::Point2f(p.x(),p.y()));
}

void DrawCorners(cv::Mat & img, std::vector<cv::Point2f> & corners,
                 const cv::Scalar & color = cv::Scalar(0,255,0))
{
    const float r = 5;
    int idx =0;
    for(auto & p:corners)
    {
        cv::Point2f pt1,pt2;
        pt1.x = p.x - r;
        pt1.y = p.y - r;
        pt2.x = p.x + r;
        pt2.y = p.y + r;
        cv::rectangle(img,pt1,pt2,color);
        cv::circle(img,p,2,color,-1);
        cv::putText(img, to_string(idx).c_str(), p - cv::Point2f(5, 5), cv::FONT_HERSHEY_PLAIN, 0.7,
                                    color, 1, 5);
        idx ++;
    }
}

void DrawForMatchResult(cv::Mat & img, const std::vector<cv::Point2f> & origin,
                        const std::vector<cv::Point2f> & result,cv::Scalar color = cv::Scalar(0,255,0))
{
    const float r = 5;
    for(int i=0;i<origin.size();i++)
    {
        cv::Point2f pt1,pt2;
        pt1.x = result[i].x - r;
        pt1.y = result[i].y - r;
        pt2.x = result[i].x + r;
        pt2.y = result[i].y + r;
        // cv::rectangle(img,pt1,pt2,color);
        cv::circle(img,result[i],3,color,-1);
        cv::line(img,origin[i],result[i],color,1);
        cv::putText(img, to_string(i).c_str(), result[i] - cv::Point2f(5, 5), cv::FONT_HERSHEY_PLAIN, 0.7,
                                    color, 1, 5);
    }
}

inline float GetPixelValue(const cv::Mat &img, float x, float y) {
    // boundary check
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x >= img.cols) x = img.cols - 1;
    if (y >= img.rows) y = img.rows - 1;
    uchar *data = &img.data[int(y) * img.step + int(x)];
    float xx = x - floor(x);
    float yy = y - floor(y);
    return float(
        (1 - xx) * (1 - yy) * data[0] +
        xx * (1 - yy) * data[1] +
        (1 - xx) * yy * data[img.step] +
        xx * yy * data[img.step + 1]
    );
}

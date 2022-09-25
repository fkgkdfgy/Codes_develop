/*
 * @Author: Liu Weilong
 * @Date: 2021-03-08 16:40:07
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-03-08 18:06:21
 * @FilePath: /3rd-test-learning/38. line_feature/plucker/code/common.h
 * @Description: 
 */
#include <iostream>
#include <vector>
#include <string>
#include <chrono>

#include "opencv2/core/core.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/line_descriptor.hpp"
#include "opencv2/features2d.hpp"

using namespace std;
using namespace cv;
using namespace cv::line_descriptor;

void DrawLine(cv::Mat & img, const std::vector<KeyLine> &lines,const  Scalar & color = Scalar(255,0,0))
{
    for(auto & line:lines)
    {
        cv::Point start(int(line.startPointX),int(line.startPointY));
        cv::Point end(int(line.endPointX),int(line.endPointY));
        cv::line(img, start, end, color,1, 8);
    }

}

void DrawMatch(cv::Mat & img1, cv::Mat & img2, const std::vector<KeyLine> & lines1,
               const vector<KeyLine> & lines2, const vector<DMatch> matches)
{
    int idx=0;
    for(auto & mt:matches)
    {
        const KeyLine & line1 =  lines1[mt.queryIdx] ;
        const KeyLine & line2 =  lines2[mt.trainIdx] ;
        
        cv::Point start1(int(line1.startPointX),int(line1.startPointY));
        cv::Point end1(int(line1.endPointX),int(line1.endPointY));
        cv::line(img1, start1, end1, Scalar(0,255,0),1, 8);
        cv::putText(img1, to_string(idx).c_str(), start1 + cv::Point(3, 3), cv::FONT_HERSHEY_PLAIN, 0.7,
                            Scalar(0,0,255), 1, 5);
        
        cv::Point start2(int(line2.startPointX),int(line2.startPointY));
        cv::Point end2(int(line2.endPointX),int(line2.endPointY));
        cv::line(img2, start2, end2, Scalar(0,255,0),1, 8);
        cv::putText(img2, to_string(idx).c_str(), start2 + cv::Point(3, 3), cv::FONT_HERSHEY_PLAIN, 0.7,
                            Scalar(0,0,255), 1, 5);
        idx++;
    }

}
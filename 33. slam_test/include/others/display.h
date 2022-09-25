/*
 * @Author: Liu Weilong
 * @Date: 2021-02-01 17:02:12
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-02-07 16:48:30
 * @FilePath: /3rd-test-learning/33. slam_test/include/others/display.h
 * @Description: 
 */
#include <vector>

#include "opencv2/core/core.hpp"
#include "opencv2/line_descriptor.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;
using namespace cv::line_descriptor;

void DrawLine(cv::Mat & img,vector<KeyLine> & lines)
{
    for(auto & e:lines)
    {
        line(img,e.getStartPoint(),e.getEndPoint(),cv::Scalar(255,255,0),2);
    }
}
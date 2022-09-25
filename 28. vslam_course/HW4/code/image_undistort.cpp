/*
 * @Author: Liu Weilong
 * @Date: 2021-02-06 22:54:35
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-02-07 22:06:05
 * @Description: 
 * 
 *               在归一化坐标系上进行矫正
 *               第一次做的有些问题应该是反向进行矫正
 * 
 * 
 */

#include <iostream>
#include <vector>
#include <string>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "Eigen/Eigen"

using namespace std;
using namespace cv;


float k1 = -0.28340811, k2 = 0.07395907, p1 = 0.00019359, p2 = 1.76187114e-05;
// 内参
float fx = 458.654, fy = 457.296, cx = 367.215, cy = 248.375;

Eigen::Vector3f PixelToNormal(int u, int v)
{
    return Eigen::Vector3f(
        (float(u)-cx)/fx,(float(v)-cy)/fy,1.0
    );
}
Eigen::Vector2i NormalToPixel(Eigen::Vector3f& p)
{
    return Eigen::Vector2i(
        p.x()*fx+cx,p.y()*fy+cy
    );
}



int main()
{
    Mat image = cv::imread("../../pictures/test.png",0);

    Mat img_undistort(image.rows,image.cols,CV_8UC1);

    int rows = image.rows, cols = image.cols;

    for (int v = 0; v < rows; v++) {
        for (int u = 0; u < cols; u++) {
        // 按照公式，计算点(u,v)对应到畸变图像中的坐标(u_distorted, v_distorted)

            Eigen::Vector3f p_d;
            auto temp = PixelToNormal(u,v);
            float r_2 = temp.x()*temp.x()+temp.y()*temp.y();
            float r_4 = r_2*r_2;
            p_d.x() = temp.x()*(1+k1*r_2+k2*r_4)+2*p1*temp.x()*temp.y()+p2*(r_2+2*temp.x()*temp.x());
            p_d.y() = temp.y()*(1+k1*r_2+k2*r_4)+2*p2*temp.x()*temp.y()+p1*(r_2+2*temp.y()*temp.y());

            int u_distorted = NormalToPixel(p_d).x(),v_distorted = NormalToPixel(p_d).y();


        // 赋值 (最近邻插值)
        if (u_distorted >= 0 && v_distorted >= 0 && u_distorted < cols && v_distorted < rows) {
            img_undistort.at<uchar>(v, u) = image.at<uchar>((int) v_distorted, (int) u_distorted);
        } else {
            img_undistort.at<uchar>(v, u) = 0;
        }
        }
    }

    imshow("raw image",image);
    imshow("undistort image",img_undistort);
    waitKey(0);


    // opencv version 
    
    return 0;
}
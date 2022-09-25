/*
 * @Author: Liu Weilong
 * @Date: 2021-02-01 16:05:00
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-02-01 16:06:41
 * @FilePath: /3rd-test-learning/33. vertical_line_slam/include/line.h
 * @Description: 
 */
#include "Eigen/Eigen"

class Line
{
    public:
    
    Eigen::Vector3d l_;
    Eigen::Vector3d m_;
};
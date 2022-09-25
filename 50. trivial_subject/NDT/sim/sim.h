/*
 * @Author: Liu Weilong
 * @Date: 2021-06-05 23:13:32
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-06-05 23:26:03
 * @Description: 
 */

#include <iostream>
#include <vector>
#include <string>
#include "pcl/registration/ndt.h"
#include "eigen3/Eigen/Eigen"

using namespace std;
using namespace Eigen;
using namespace pcl;

using PointType = pcl::PointXYZI;
using ColorPointType = pcl::PointXYZRGB;

using PointCloud = pcl::PointCloud<PointType>;
using PointCloudPtr = pcl::PointCloud<PointType>::Ptr;

using ColorPointCloud = pcl::PointCloud<ColorPointType>;
using ColorPointCloudPtr = pcl::PointCloud<ColorPointType>::Ptr;


class NDTSimEigen
{
    public:

    class SimPointCloud
    {
        public:
        bool line ;
        bool only_outlier;
        Eigen::Vector2d start;
        Eigen::Vector2d end;
        Eigen::Matrix2d sigma;
        double density
        double pi;
    };

    void AddNewPointCloud(SimPointCloud & pc) const;
    void PointCloudShow() const;

    private:
    
    std::vector<SimPointCloud> sim_point_clouds_;
    PointCloud
};







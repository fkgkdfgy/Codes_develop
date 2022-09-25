/*
 * @Author: Liu Weilong
 * @Date: 2020-11-04 05:27:33
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-05-27 15:41:19
 * @Description:  
 *               主要是用于 Target 和 Source 的对比展示
 */
#include "display/display.hpp"
#include "pcl/io/pcd_io.h"

int main()
{
    pcl::PointCloud<pcl::PointXYZ>::Ptr target_point_cloud(new pcl::PointCloud<pcl::PointXYZ>());
    pcl::PointCloud<pcl::PointXYZ>::Ptr source_point_cloud(new pcl::PointCloud<pcl::PointXYZ>());
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr target_cloud_with_color_(new pcl::PointCloud<pcl::PointXYZRGB>());
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr source_cloud_with_color_(new pcl::PointCloud<pcl::PointXYZRGB>());
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr whole_cloud_with_color_(new pcl::PointCloud<pcl::PointXYZRGB>());
    pcl::io::loadPCDFile("/home/lwl/workspace/3rd-test-learning/8. ceres/ceres_self_example/source_point_cloud.pcd",*source_point_cloud);
    pcl::io::loadPCDFile("/home/lwl/workspace/3rd-test-learning/8. ceres/ceres_self_example/target_point_cloud.pcd",*target_point_cloud);

    AssignColorToPointCloud(target_point_cloud,target_cloud_with_color_,0,255,0);
    AssignColorToPointCloud(source_point_cloud,source_cloud_with_color_,0,0,255);

    *whole_cloud_with_color_ = *target_cloud_with_color_ + *source_cloud_with_color_;

    SimpleShow(whole_cloud_with_color_);
    SimpleShow(target_cloud_with_color_);
    SimpleShow(source_cloud_with_color_);
}
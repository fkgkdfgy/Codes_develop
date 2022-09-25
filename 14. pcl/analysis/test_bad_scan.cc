/*
 * @Author: Liu Weilong
 * @Date: 2021-05-27 17:09:04
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-06-01 10:32:44
 * @Description: 
 * 
 * 主要是用于测试处理之后的点云，是否会出现跳变的问题
 * +
 * 尝试权重对整体匹配的影响,真的是人傻了
 * 
 */

#include <iostream>
#include <string>
#include <vector>

#include "eigen3/Eigen/Eigen"
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <pcl/visualization/cloud_viewer.h>
#include <pcl/io/pcd_io.h>
#include <pcl/filters/voxel_grid_covariance.h>

#include "ndt.h"
using namespace std;

namespace fs = boost::filesystem;  
namespace po = boost::program_options;

using PC = pcl::PointCloud<pcl::PointXYZI>;


template<typename TPtr>
void SimpleShow(TPtr & point_cloud_)
{
    pcl::visualization::CloudViewer viewer ("Simple Cloud Viewer");
    viewer.showCloud (point_cloud_);
    while (!viewer.wasStopped ())
    {
    }
}

template<typename TPtrIn,typename TPtrOut>
void AssignColorToPointCloud(TPtrIn & pc_in, TPtrOut & pc_out, uint8_t r,uint8_t g,uint8_t b)
{
    pcl::copyPointCloud(*pc_in,*pc_out);
    for (int i =0;i<pc_out->size();i++)
    {
        uint32_t rgb = ((uint32_t)r << 16 | (uint32_t)g << 8 | (uint32_t)b);
        pc_out->points[i].rgb = *reinterpret_cast<float*>(&rgb);
    }
}

Eigen::Matrix4f v3f2mat4f(const Eigen::Vector3f& t)
{
	float cx = cos(t(2));
	float sx = sin(t(2));
	float tx = t(0);
	float ty = t(1);
	Eigen::Matrix4f res(Eigen::Matrix4f::Identity());
	res.block<2, 2>(0, 0) << cx, -sx,
							sx, cx;
	res.block<2, 1>(0, 3) << tx,
							ty;
							
	return res;
}

int main()
{
    pcl::PointCloud<pcl::PointXYZI>::Ptr map (new pcl::PointCloud<pcl::PointXYZI>());
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr map_color (new pcl::PointCloud<pcl::PointXYZRGB>());
    pcl::io::loadPCDFile("/home/lwl/sany_project/sany_agv_slam/map/slam_map.pcd",*map);
    pcl::PointCloud<pcl::PointXYZI>::Ptr scan (new pcl::PointCloud<pcl::PointXYZI>());
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr scan_color (new pcl::PointCloud<pcl::PointXYZRGB>());

    pcl::PointCloud<pcl::PointXYZI>::Ptr scan1 (new pcl::PointCloud<pcl::PointXYZI>());
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr scan1_color (new pcl::PointCloud<pcl::PointXYZRGB>());

    pcl::PointCloud<pcl::PointXYZI>::Ptr scan2 (new pcl::PointCloud<pcl::PointXYZI>());
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr scan2_color (new pcl::PointCloud<pcl::PointXYZRGB>());

    pcl::PointCloud<pcl::PointXYZI>::Ptr trans_scan (new pcl::PointCloud<pcl::PointXYZI>());

    pcl::io::loadPCDFile("/home/lwl/sany_project/sany_agv_slam/lwl_test/200.pcd",*scan);
    pcl::io::loadPCDFile("/home/lwl/sany_project/sany_agv_slam/lwl_test/200_3.pcd",*scan1); // 修改程度最大的帧
    pcl::io::loadPCDFile("/home/lwl/sany_project/sany_agv_slam/lwl_test/200_2.pcd",*scan2); // 修改程度稍小的帧


    // SimpleShow(scan);
    SimpleShow(scan1);
    
    Eigen::Vector3f real(-0.649424,5.29211,-1.57293);
    auto real_matrix=v3f2mat4f(real);

    pcl::transformPointCloud(*scan,*trans_scan,real_matrix);
    

    ndt_analysis::NormalDistributionsTransform<pcl::PointXYZI,pcl::PointXYZI> ndt_;
    
    ndt_.setResolution(0.5);
    ndt_.setMaximumIterations(20);
    ndt_.setStepSize(0.02);
    ndt_.setTransformationEpsilon(0.01);
    ndt_.setInputTarget(map);
    ndt_.setInputSource(scan);
    ndt_.align(*trans_scan,real_matrix);


    cout<<"the estimation:"<<ndt_.getFinalTransformation().col(3).transpose()<<endl;

    
    AssignColorToPointCloud(map,map_color,255,255,255);
    AssignColorToPointCloud(trans_scan,scan_color,255,0,0);

    pcl::PointCloud<pcl::PointXYZRGB>::Ptr all (new pcl::PointCloud<pcl::PointXYZRGB>());

    * all = * map_color + *scan_color;
    
    // SimpleShow(all);

    ndt_.setInputSource(scan1);
    ndt_.align(*trans_scan,real_matrix);
    cout<<"the estimation:"<<ndt_.getFinalTransformation().col(3).transpose()<<endl;

    // ndt_.setInputTarget(scan);
    // ndt_.setInputSource(scan1);
    // ndt_.align(*trans_scan,real_matrix);
    // cout<<"the estimation:"<<ndt_.getFinalTransformation().col(3).transpose()<<endl;


    AssignColorToPointCloud(map,map_color,255,255,255);
    AssignColorToPointCloud(trans_scan,scan_color,255,0,0);

    * all = * map_color + *scan_color;
    
    // SimpleShow(all);

    // 尝试权重对匹配结果的影响

    // pcl::PointCloud<pcl::PointXYZI>::Ptr all_without_color (new pcl::PointCloud<pcl::PointXYZI>());
    // *all_without_color = *scan+*scan1; 
    // for(int i = 0;i<2;i++)
    // {
    //     *all_without_color += *scan1;
    // }

    // ndt_.setInputSource(all_without_color);
    // ndt_.align(*trans_scan,real_matrix);
    // cout<<"the estimation:"<<ndt_.getFinalTransformation().col(3).transpose()<<endl;
}
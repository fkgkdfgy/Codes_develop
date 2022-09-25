/*
 * @Author: Liu Weilong
 * @Date: 2021-05-27 17:09:04
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-06-01 19:09:16
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

using Cloud = pcl::PointCloud<pcl::PointXYZI>;
using CloudPtr = pcl::PointCloud<pcl::PointXYZI>::Ptr;
using ColorCloud = pcl::PointCloud<pcl::PointXYZRGB>;
using ColorCloudPtr = pcl::PointCloud<pcl::PointXYZRGB>::Ptr;

class SimpleUtils
{
    public:
    SimpleUtils(){
        path = "/home/lwl/sany_project/sany_agv_slam/lwl_test/pcd/";
        local_map_path = "/home/lwl/sany_project/sany_agv_slam/lwl_test/local_map/";
        suffix = ".pcd";
        fout1.open("/home/lwl/sany_project/sany_agv_slam/lwl_test/result/result.txt");
        fout2.open("/home/lwl/sany_project/sany_agv_slam/lwl_test/result/result_local.txt");
        if(!fout1.is_open()){
            std::cerr<<"[FATAL]: Wrong filename path"<<std::endl;
            abort();
        }
        if(!fout2.is_open()){
        std::cerr<<"[FATAL]: Wrong filename path"<<std::endl;
        abort();
        }
        std::cout<<"AAAAAAA"<<std::endl;

        whole_point_cloud.reset(new ColorCloud());
    }
    
    bool GetANewPCDFileName(std::string & filename)
    {
        if(count<thres) 
        {
            filename = path + std::to_string(count)+suffix;
            count++;
            return true;
        }
        return false;
    }

    bool GetANewLocalMapPCDFileName(std::string & filename)
    {
        if(local_map_count<local_map_thres)
        {
            filename = local_map_path+ std::to_string(local_map_count)+suffix;
            local_map_count++;
            return true;
        }
        return false;
    }

    bool SaveResult()
    {
        if(fout1.is_open())
        {
            for(auto & r :results)
            fout1<<r.transpose()<<std::endl;
            
        }
 
        if(fout2.is_open())
        {
            for(auto & r :local_results)
            fout2<<r.transpose()<<std::endl;
        }
    }

    void ResetWholePointCloud(){whole_point_cloud->clear();}

    void AddNewPointCloud(const CloudPtr scan, uint8_t r = 255,uint8_t g=255,uint8_t b=255)
    {
        ColorCloudPtr tmp (new ColorCloud());
        AssignColorToPointCloud(scan,tmp,r,g,b);
        *whole_point_cloud += *tmp;
    }

    void ShowWholePointCloud(){
        SimpleShow(whole_point_cloud);
    }
    
    std::string path,suffix,local_map_path;
    int count =1;
    int thres =1824;
    int local_map_count = 0;
    int local_map_thres = 25;
    std::vector<Eigen::Vector3f> results;
    std::vector<Eigen::Vector3f> local_results;
    std::vector<Eigen::Vector3f> map2map_results;
    std::fstream fout1;
    std::fstream fout2;

    ColorCloudPtr whole_point_cloud;

    
};

int main(int argc,char ** argv)
{
    pcl::PointCloud<pcl::PointXYZI>::Ptr map (new pcl::PointCloud<pcl::PointXYZI>());
    pcl::PointCloud<pcl::PointXYZI>::Ptr tmp (new pcl::PointCloud<pcl::PointXYZI>());
    pcl::io::loadPCDFile("/home/lwl/sany_project/sany_agv_slam/map/slam_map.pcd",*map);

    ndt_analysis::VoxelGridCovariance<pcl::PointXYZI> vg;
    vg.setMinPointPerVoxel(6);
    double resolution = atof(argv[1]);
    vg.setLeafSize(resolution,resolution,resolution);

    SimpleUtils su;
    su.ResetWholePointCloud();
    vg.setInputCloud(map);

    vg.filter(*tmp);
    
    su.AddNewPointCloud(map,255,255,255);

    su.ShowWholePointCloud();

    vg.getLeafPointCloud(*tmp);

    su.AddNewPointCloud(tmp,255,0,0);

    su.ShowWholePointCloud();
    
    pcl::io::savePCDFile("/home/lwl/sany_project/sany_agv_slam/map/slam_map_new.pcd",*tmp);
}
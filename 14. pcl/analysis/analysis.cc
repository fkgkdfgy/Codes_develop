/*
 * @Author: Liu Weilong
 * @Date: 2021-05-27 15:32:23
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-05-27 16:50:39
 * @Description: 
 */

#include <iostream>
#include <string>
#include <vector>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <pcl/visualization/cloud_viewer.h>
#include <pcl/io/pcd_io.h>
#include <pcl/filters/voxel_grid_covariance.h>

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


Eigen::Vector3f mat4f2v3f(const Eigen::Matrix4f& T)
{
    return Eigen::Vector3f(T(0, 3),
                           T(1, 3),
                           atan2f(T(1, 0), T(0, 0)));
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

int get_filenames(const std::string& dir, std::vector<std::string>& filenames)   //获取文件名和个数
{  
    fs::path path(dir);  
    if (!fs::exists(path))  //判断文件路径是否为空
    {  
        return -1;  
    }  
    
    fs::directory_iterator end_iter;  
    for (fs::directory_iterator iter(path); iter!=end_iter; ++iter)  
    {
        if (fs::is_regular_file(iter->status()))  
        {  
            filenames.push_back(iter->path().string()); 
        }  
      
        if (fs::is_directory(iter->status()))  
        {  
            get_filenames(iter->path().string(), filenames);//是目录则递归  
        }  
    }  
     
    return filenames.size();  //返回文件大小
} 

int main()
{
    vector<string> filenames;
    get_filenames("/home/lwl/sany_project/sany_agv_slam/lwl_test/pcd",filenames);
    pcl::PointCloud<pcl::PointXYZI>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZI>());

    // view map
    {
        pcl::visualization::CloudViewer viewer_map ("Simple Cloud Viewer");
        pcl::io::loadPCDFile("/home/lwl/sany_project/sany_agv_slam/map/slam_map.pcd",*cloud);
        viewer_map.showCloud(cloud);
        while(!viewer_map.wasStopped()){}
    }

    
    // view VoxelGrid Filter
    pcl::VoxelGridCovariance<pcl::PointXYZI> vg;
    vg.setMinPointPerVoxel(100);
    vg.setLeafSize(0.5,0.5,0.5);
    vg.setInputCloud(cloud);
    pcl::PointCloud<pcl::PointXYZI>::Ptr cloud_out (new pcl::PointCloud<pcl::PointXYZI>());
    vg.filter(*cloud_out);
    
    {
        pcl::visualization::CloudViewer viewer_map ("Simple Cloud Viewer");
        pcl::io::loadPCDFile("/home/lwl/sany_project/sany_agv_slam/map/slam_map.pcd",*cloud);
        viewer_map.showCloud(cloud_out);
        while(!viewer_map.wasStopped()){}
    }



    int count = 1;
    int size = filenames.size();
    for(int i=0;i<size;i++)
    {
        pcl::visualization::CloudViewer viewer ("Simple Cloud Viewer");
        cloud->clear();
        if(filenames[count].substr(filenames[count].length()-7,3)!= "pre")
        {
            pcl::io::loadPCDFile(filenames[count],*cloud);
            viewer.showCloud(cloud);
            while(!viewer.wasStopped()){}
        }
        count++;
    }
}



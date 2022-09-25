/*
 * @Author: Liu Weilong
 * @Date: 2020-10-25 17:08:16
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2020-10-25 22:43:12
 * @Description:  
 */
#include <string>
#include <iostream>
#include <sstream>
#include <pcl/io/pcd_io.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/common/transforms.h>
#include <pcl/registration/ndt.h>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include "omp.h"
#include "lidar_localization/mapping/loop_closing/Scancontext.h"

namespace fs = boost::filesystem;  
namespace po = boost::program_options;

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

bool JointMap(int index,
              const std::vector<Eigen::Matrix4f> & pose_array,
              pcl::PointCloud<pcl::PointXYZ>::Ptr submap_ptr)
{
    int extend_scan_num =3;
    pcl::VoxelGrid<pcl::PointXYZ> voxel_filter_inner;
    voxel_filter_inner.setLeafSize(0.6,0.6,0.6);
    
    if(index<extend_scan_num)
    {
        std::cout<<"considering the boundary condition ,then return false"<<std::endl;
    }
    
    auto T_world_to_index = pose_array[index].inverse();
    submap_ptr->clear();

    for(int i = index-extend_scan_num;i<=index+extend_scan_num;i++)
    {
        auto T_i_to_index = T_world_to_index * pose_array[i];
        pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_tmp(new pcl::PointCloud<pcl::PointXYZ>());
        std::string path = "/home/lwl/workspace/chapter_2/src/lidar_localization/slam_data/key_frames";
        std::string filename = path + "/key_frame_" + std::to_string(i) +".pcd";
        pcl::io::loadPCDFile(filename,*cloud_tmp);
        pcl::transformPointCloud(*cloud_tmp,*cloud_tmp,T_i_to_index);
        *submap_ptr += *cloud_tmp;
    }

    voxel_filter_inner.setInputCloud(submap_ptr);
    voxel_filter_inner.filter(*submap_ptr);
}

bool readKittiPose(std::string pose_path , std::vector<Eigen::Matrix4f> & pose)
{
    std::ifstream ofs(pose_path);
    if (!ofs.is_open())
    {
        return false;
    }
    std::string one_pose;
   
    if(pose.size() !=0)
    return false;
    std::stringstream ofss;
    while(getline(ofs,one_pose))
    {
        // std::cout<<one_pose <<std::endl;
        ofss.str(one_pose);
        Eigen::Matrix4f new_pose = Eigen::Matrix4f::Identity();
        for(int i =0;i<3;i++)
        {
            for(int j =0;j<4;j++)
            {
                float tmp;
                ofss>>tmp;
                new_pose(i,j) = tmp; 
            }
        }
        ofss.clear();
        pose.push_back(new_pose);
    }
    return true;
    
}

int main()
{
    SCManager sc_manager;
    pcl::VoxelGrid<pcl::PointXYZ> voxel_filter;
    voxel_filter.setLeafSize(0.6,0.6,0.6);
    std::vector<std::string> filename_array;
    std::string path = "/home/lwl/workspace/chapter_2/src/lidar_localization/slam_data/key_frames";
    auto file_num = get_filenames(path,filename_array);
    
    // 保存一下数值 这里直接把122这个帧 剃了出去
    pcl::PointCloud<pcl::PointXYZ>::Ptr scan_ptr (new pcl::PointCloud<pcl::PointXYZ>());
    
    for(int i = 0;i<122;i++)
    {
        std::string filename = path + "/key_frame_" + std::to_string(i) +".pcd";
        pcl::io::loadPCDFile(filename,*scan_ptr);
        voxel_filter.setInputCloud(scan_ptr);
        voxel_filter.filter(*scan_ptr);
        sc_manager.makeAndSaveScancontextAndKeys(*scan_ptr);
    }

        for(int i = 123;i<1500;i++)
    {
        std::string filename = path + "/key_frame_" + std::to_string(i) +".pcd";
        pcl::io::loadPCDFile(filename,*scan_ptr);
        voxel_filter.setInputCloud(scan_ptr);
        voxel_filter.filter(*scan_ptr);
        sc_manager.makeAndSaveScancontextAndKeys(*scan_ptr);
    }

    int test_scan_index  = 123;
    
    std::string filename = path + "/key_frame_" + std::to_string(test_scan_index) +".pcd";
    pcl::io::loadPCDFile(filename,*scan_ptr);
    sc_manager.makeAndSaveScancontextAndKeys(*scan_ptr);

    std::string ground_path = "/home/lwl/workspace/chapter_2/src/lidar_localization/slam_data/trajectory/ground_truth.txt";
    
    std::vector<Eigen::Matrix4f> pose_array;
    
    readKittiPose(ground_path,pose_array);
    
    auto laser_122 = pose_array[122];

    pcl::PointCloud<pcl::PointXYZ>::Ptr scan_122(new pcl::PointCloud<pcl::PointXYZ>);


    filename = path + "/key_frame_" + std::to_string(122) +".pcd";
    pcl::io::loadPCDFile(filename,*scan_122);
    
    voxel_filter.setInputCloud(scan_122);
    voxel_filter.filter(*scan_122);
    sc_manager.makeAndSaveScancontextAndKeys(*scan_122);
    // sc_manager.makeAndSaveScancontextAndKeys(*scan_122);
    auto result = sc_manager.detectLoopClosure();
    
    if(result.size() == 0)
    {
        std::cout<<"the result is empty"<<std::endl;
        return 0;
    }

    
    std::vector<float> score_array;
    std::vector<Eigen::Matrix4f> transform_array;
    score_array.resize(result.size(),99999);
    transform_array.resize(result.size(),Eigen::Matrix4f::Zero());



    const float NDTFitnessScore = 5;

    omp_set_num_threads(6);
    #pragma omp parallel for
    for(int i= 0;i<result.size();i++)
    {
        auto front = result[i];
        pcl::PointCloud<pcl::PointXYZ>::Ptr submap(new pcl::PointCloud<pcl::PointXYZ>());
        pcl::PointCloud<pcl::PointXYZ>::Ptr nothing(new pcl::PointCloud<pcl::PointXYZ>());
        JointMap(front.first,pose_array,submap);

        
        filename = path + "/key_frame_" + std::to_string(122) +".pcd";
        pcl::io::loadPCDFile(filename,*scan_ptr);
        voxel_filter.setInputCloud(scan_ptr);
        voxel_filter.filter(*scan_ptr);
        
        pcl::NormalDistributionsTransform<pcl::PointXYZ,pcl::PointXYZ> ndt_;
        ndt_.setResolution(1);
        ndt_.setMaximumIterations(20);
        ndt_.setStepSize(0.1);
        ndt_.setTransformationEpsilon(0.01);
        ndt_.setInputTarget(submap);
        ndt_.setInputSource(scan_ptr);
        ndt_.align(*nothing);

        score_array[i]= ndt_.getFitnessScore();
        transform_array[i] = ndt_.getFinalTransformation();

        
    }

    int best_fit_index = 0;
    float best_score = 9999999;
    for(int i=0;i<score_array.size();i++)
    {
        if(score_array[i]<best_score)
        {
            best_fit_index = i;
            best_score = score_array[i];
        }
    }

    std::cout<<best_fit_index<<std::endl;
    std::cout<<"the more fitable transform is "<<std::endl
             << pose_array[result[best_fit_index].first]*transform_array[best_fit_index]<<std::endl
             << "the corresponding score is "<<std::endl
             << score_array[best_fit_index]<<std::endl
             << " the real 122  transform is "<<std::endl
             << pose_array[122]<<std::endl;
            
    return 0;
}
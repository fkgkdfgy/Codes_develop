/**
 *  读取标准形式的数据并进行保存
 * 
 * 
*/
#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Eigen/Eigen"

#include "pcl/io/pcd_io.h"
#include "pcl/registration/gicp.h"

using namespace std;
using PointCloud = pcl::PointCloud<pcl::PointXYZI>;
using PointCloudPtr = pcl::PointCloud<pcl::PointXYZI>::Ptr;

struct KFInfo
{
    PointCloudPtr point_cloud;
    unsigned int id;
    Eigen::Vector3d pose;
};

Eigen::Matrix4f GetTransformMatrixFromxytheta(const Eigen::Vector3f &xytheta)
{
    Eigen::Matrix4f pose = Eigen::Matrix4f::Identity();

    pose(0, 0) = cos(xytheta.z());
    pose(0, 1) = -sin(xytheta.z());
    pose(1, 0) = sin(xytheta.z());
    pose(1, 1) = cos(xytheta.z());

    pose(0, 3) = xytheta.x();
    pose(1, 3) = xytheta.y();

    return pose;
}

class KFAdjustDataLoader
{
    public:

    void Load(const string & input_path)
    {
        ifstream input;

        string pose_text = input_path + "/test_bag/pose.txt";
        cout<<pose_text<<endl;
        input.open(pose_text,fstream::in);
        if(!input.is_open()){cout<<"KFAdjustDataLoader: check input path"<<endl; return;}   
        stringstream ss;

        string data_line;
        double data[3];
        while(getline(input,data_line))
        {
            string info;
            ss.str(data_line);
            string line_small;
            getline(ss,line_small,' ');
            pcd_name.push_back(line_small);
            for (int i=0;i<3;i++)
            {
                getline(ss,line_small,' ');
                data[i] = atof(line_small.c_str());
            }
            Eigen::Vector3d pose;
            pose << data[0]+2.0,data[1]+0.5,data[2];
            kfs_pose.push_back(pose);
            ss.clear();
        }
        GetAllKFInfo();
    }

    void GetAllKFInfo(){
        int count = 0;
        
        point_clouds.resize(pcd_name.size());
        string pcd_path;
        while(count<pcd_name.size())
        {
            kfs_idx.push_back(count);
            pcd_path = bag_path+"/test_bag/"+pcd_name[count]+".pcd";
            point_clouds[count].reset(new PointCloud());
            pcl::io::loadPCDFile(pcd_path.c_str(),*point_clouds[count]);
            Eigen::Matrix4f transform = GetTransformMatrixFromxytheta(kfs_pose[count].cast<float>());
            pcl::transformPointCloud(*point_clouds[count],*point_clouds[count],transform);
            count += interval;
        }
    }

    bool GetNextKFInfo(KFInfo & result){
        if(kf_count<kfs_idx.size())
        {
            result.id = kf_count;
            unsigned int tmp = kfs_idx[kf_count];
            result.point_cloud =point_clouds[tmp];
            result.pose = kfs_pose[tmp];
            kf_count++;
            return true;
        }
        return false;
    }

    bool GetRawDataKFInfoFromIndex(int idx,KFInfo & result)
    {
        if(idx<0&&idx>=kfs_pose.size()) return false;
        result.id = idx;
        int raw_idx = kfs_idx[idx];
        result.point_cloud.reset(new PointCloud());
        pcl::io::loadPCDFile(bag_path+"/test_bag/"+pcd_name[raw_idx]+".pcd",*result.point_cloud);
        result.pose = kfs_pose[raw_idx];
        return true;
    }

    int interval = 5;
    int kf_count = 0;
    string bag_path;
    vector<string> pcd_name;
    vector<PointCloudPtr> point_clouds;
    vector<Eigen::Vector3d> kfs_pose;
    vector<unsigned int> kfs_idx;
    
};
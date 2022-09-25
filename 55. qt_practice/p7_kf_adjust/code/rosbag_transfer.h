/**
 * 1. 把rosbag 保存成标准读取形式
 * 
 * 目前，默认rosbag 含有/ground_truth/state /scan 两个内容
 * 
 * 输出:
 * 1. ${timestamp}.pcd 
 * 2. pose.txt 使用 scan_timestamp x y theta 的方式进行排列
 * 
*/

#include <vector>
#include <string>
#include <algorithm>
#include <fstream>

#include "laser_geometry/laser_geometry.h"
#include "pcl_conversions/pcl_conversions.h"
#include "pcl/conversions.h"
#include "rosbag/bag.h"
#include "rosbag/view.h"
#include "nav_msgs/Odometry.h"
#include "sensor_msgs/LaserScan.h"
#include "sensor_msgs/PointCloud2.h"
#include "Eigen/Eigen"

using namespace std;

using PointCloud = pcl::PointCloud<pcl::PointXYZI>;
using PointCloudPtr = pcl::PointCloud<pcl::PointXYZI>::Ptr;

struct GTState
{
    double timestamp;
    Eigen::Vector3d pose;
};

void RosbagTransfer(const string & rosbag_path, const string & standard_output)
{
    rosbag::Bag input;
    input.open(rosbag_path.c_str(),rosbag::BagMode::Read);
    vector<string> topics={"/ground_truth/state","/scan"};
    rosbag::View view(input,rosbag::TopicQuery(topics));
    
    vector<double> scan_timestamp;
    vector<GTState> gts;

    laser_geometry::LaserProjection projector;

    cout<<"Processing scan ...." << endl;
    
    for(auto element:view)
    {
        sensor_msgs::LaserScan::ConstPtr point_cloud = element.instantiate<sensor_msgs::LaserScan>();
        if(point_cloud !=NULL)
        {
            sensor_msgs::PointCloud2 ros_point_cloud;
            projector.projectLaser(*point_cloud,ros_point_cloud);
            double timestamp = point_cloud->header.stamp.toSec();
            scan_timestamp.push_back(timestamp);
            PointCloud pcl_point_cloud;
            pcl::fromROSMsg(ros_point_cloud,pcl_point_cloud);
            pcl::io::savePCDFileBinary(standard_output+"/"+to_string(timestamp)+".pcd",pcl_point_cloud);
        }
    }

    cout<<"Processing groundtruth ...." << endl;

    for(auto element:view)
    {
        nav_msgs::Odometry::ConstPtr gt = element.instantiate<nav_msgs::Odometry>();
        if(gt!=NULL)
        {
            GTState gt_state;
            
            double timestamp = gt->header.stamp.toSec();
            Eigen::Vector3d eigen_gt;
            eigen_gt<< gt->pose.pose.position.x,
                       gt->pose.pose.position.y,
                       gt->pose.pose.orientation.z;

            gt_state.timestamp = timestamp;
            gt_state.pose = eigen_gt;

            gts.push_back(gt_state);
        }
    }

    std::sort(scan_timestamp.begin(),scan_timestamp.end(),less<double>());
    std::sort(gts.begin(),gts.end(),[](const GTState & a, const GTState & b)-> bool{
        return a.timestamp < b.timestamp;
    });

    vector<GTState> result;

    cout<<"Generating groundtruth ...." << endl;

    bool find = false;
    int count =0;
    for(double st:scan_timestamp)
    {
        find = false;
        for(const auto & gt:gts)
        {
            if(gt.timestamp>st)
            {
                GTState new_gt;
                new_gt.timestamp = st;
                new_gt.pose = gt.pose;
                result.push_back(new_gt);
                find = true;
                break;
            }
        }
        count++;
        if(!find){
            GTState new_gt;
            new_gt.timestamp = st;
            new_gt.pose = gts.back().pose;
            result.push_back(new_gt);
            break;
        }
    }

    cout<<"the total size :" << scan_timestamp.size()<<"   the count size:"<< count<<endl;

    string pose_out_path = standard_output + "/pose.txt";
    ofstream out;
    out.open(pose_out_path,fstream::out|fstream::trunc);

    for(const auto & gt:result)
    {
        out<<to_string(gt.timestamp)<<" "<< gt.pose.x()<<" "<<gt.pose.y()<<" "<<gt.pose.z()<<endl;
    }

    out.close();
}
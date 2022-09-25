/*
 * @Author: Liu Weilong
 * @Date: 2020-10-24 10:32:45
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2020-10-25 15:09:27
 * @Description: LoopAdapter
 */


#include "lidar_localization/mapping/loop_closing/loop_closing.hpp"

#include <cmath>
#include <algorithm>
#include <pcl/common/transforms.h>
#include <pcl/io/pcd_io.h>
#include "glog/logging.h"

#include "lidar_localization/global_defination/global_defination.h"
#include "lidar_localization/models/registration/ndt_registration.hpp"
#include "lidar_localization/models/cloud_filter/voxel_filter.hpp"
#include "lidar_localization/models/cloud_filter/no_filter.hpp"
#include "lidar_localization/tools/print_info.hpp"

#include "lidar_localization/mapping/loop_closing/sc_loop.h"
namespace lidar_localization
{

    bool SCLoopAdapter::InitWithConfig()
    {
        std::string config_file_path = WORK_SPACE_PATH + "/config/mapping/loop_closing.yaml";
        YAML::Node config_node = YAML::LoadFile(config_file_path);

        std::cout << "-----------------闭环检测初始化-------------------" << std::endl;
        InitParam(config_node); // TODO 这个地方还还有进行处理
        InitDataPath(config_node);
        InitRegistration(registration_ptr_, config_node);
        InitFilter("map", map_filter_ptr_, config_node);
        InitFilter("scan", scan_filter_ptr_, config_node);
    }

    bool SCLoopAdapter::InitParam(const YAML::Node& config_node)
    {
        extend_frame_num_ = config_node["extend_frame_num"].as<int>();
        loop_step_ = config_node["loop_step"].as<int>();
        diff_num_ = config_node["diff_num"].as<int>();
        detect_area_ = config_node["detect_area"].as<float>();
        fitness_score_limit_ = config_node["fitness_score_limit"].as<float>();

        return true;
    }

    bool SCLoopAdapter::InitDataPath(const YAML::Node& config_node)
    {
        std::string data_path = config_node["data_path"].as<std::string>();
        if (data_path == "./") {
            data_path = WORK_SPACE_PATH;
        }

        key_frames_path_ = data_path + "/slam_data/key_frames";

        return true;
    }

    bool SCLoopAdapter::InitRegistration(std::shared_ptr<RegistrationInterface>& registration_ptr, const YAML::Node& config_node)
    {
        std::string registration_method = config_node["registration_method"].as<std::string>();
        std::cout << "闭环点云匹配方式为：" << registration_method << std::endl;

        if (registration_method == "NDT") {
            registration_ptr = std::make_shared<NDTRegistration>(config_node[registration_method]);
        } else {
            LOG(ERROR) << "没找到与 " << registration_method << " 相对应的点云匹配方式!";
            return false;
        }
        return true;
    }

    bool SCLoopAdapter::InitFilter(std::string filter_user, 
                                   std::shared_ptr<CloudFilterInterface>& filter_ptr, 
                                   const YAML::Node& config_node)
    {
        std::string filter_mothod = config_node[filter_user + "_filter"].as<std::string>();
        std::cout << "闭环的" << filter_user << "选择的滤波方法为：" << filter_mothod << std::endl;

        if (filter_mothod == "voxel_filter") {
            filter_ptr = std::make_shared<VoxelFilter>(config_node[filter_mothod][filter_user]);
        } else if (filter_mothod == "no_filter") {
            filter_ptr =std::make_shared<NoFilter>();
        } else {
            LOG(ERROR) << "没有为 " << filter_user << " 找到与 " << filter_mothod << " 相对应的滤波方法!";
            return false;
        }
        return true;
    }
    

    bool SCLoopAdapter::Update(const KeyFrame key_frame, const KeyFrame key_gnss)
    {
        has_new_loop_pose_ = false;

        all_key_frames_.push_back(key_frame);
        all_key_gnss_.push_back(key_gnss);

        UpdateSCManager();

        int key_frame_index = 0;
        float yaw_predict = 0;
        if (!DetectNearestKeyFrame(key_frame_index,yaw_predict))
            return false;

        if (!CloudRegistration(key_frame_index,yaw_predict))
            return false;

        has_new_loop_pose_ = true;
        return true;
    }



    bool SCLoopAdapter::UpdateSCManager()
    {   
        pcl::PointCloud<pcl::PointXYZ>::Ptr scan_cloud_ptr(new pcl::PointCloud<pcl::PointXYZ>());

        std::string file_path = key_frames_path_ + "/key_frame_" + std::to_string(all_key_frames_.back().index) + ".pcd";
        pcl::io::loadPCDFile(file_path, *scan_cloud_ptr);
        scan_filter_ptr_->Filter(scan_cloud_ptr, scan_cloud_ptr);

        // 添加旋转 让得到的raw就是有意义的了 transform 里面是存在问题平移的这样就肯定会出问题
        // pcl::transformPointCloud(*scan_cloud_ptr,*scan_cloud_ptr,all_key_frames_.back().pose);

        sc_manager_.makeAndSaveScancontextAndKeys(*scan_cloud_ptr);
    }

    bool SCLoopAdapter::DetectNearestKeyFrame(int & key_frame_index,float& yaw_predict)
    {
        static int skip_cnt = 0;
        static int skip_num = loop_step_;
        if (++skip_cnt < skip_num)
            return false;

        if ((int)all_key_gnss_.size() < diff_num_ + 1)
            return false;

        // int key_num = (int)all_key_gnss_.size();
        // float min_distance = 1000000.0;
        // float distance = 0.0;

        // KeyFrame history_key_frame;
        // KeyFrame current_key_frame = all_key_gnss_.back();

        // key_frame_index = -1;
        // for (int i = 0; i < key_num - 1; ++i) {

        //     if (key_num - i < diff_num_)
        //         break;
            
        //     history_key_frame = all_key_gnss_.at(i);
        //     distance = fabs(current_key_frame.pose(0,3) - history_key_frame.pose(0,3)) + 
        //             fabs(current_key_frame.pose(1,3) - history_key_frame.pose(1,3)) + 
        //             fabs(current_key_frame.pose(2,3) - history_key_frame.pose(2,3));

        //     if (distance < min_distance) {
        //         min_distance = distance;
        //         key_frame_index = i;
        //     }
            
        // }

        auto result = sc_manager_.detectLoopClosureID();
        
        key_frame_index = result.first;
        yaw_predict = result.second;

        if (key_frame_index < extend_frame_num_ || key_frame_index == -1)
            return false;

        // skip_cnt = 0;
        // skip_num = (int)min_distance;
        // if (min_distance > detect_area_) {
        //     skip_num = std::max((int)(min_distance / 2.0), loop_step_);
        //     return false;
        // } else {
        //     skip_num = loop_step_;
        //     return true;
        // }
        LOG(INFO)<<"[Loop Found]: here we found a loop closure between "<<all_key_frames_.size()<<" and "<<key_frame_index;
        skip_cnt = 0;
        skip_num = loop_step_;
        return true;
    }

bool SCLoopAdapter::CloudRegistration(int key_frame_index ,float yaw_predict) {
    // 生成地图
    CloudData::CLOUD_PTR map_cloud_ptr(new CloudData::CLOUD());
    Eigen::Matrix4f map_pose = Eigen::Matrix4f::Identity();
    JointMap(key_frame_index, map_cloud_ptr, map_pose);

    // 生成当前scan
    CloudData::CLOUD_PTR scan_cloud_ptr(new CloudData::CLOUD());
    Eigen::Matrix4f scan_pose = Eigen::Matrix4f::Identity();
    JointScan(scan_cloud_ptr, scan_pose);

    // 匹配
    Eigen::Matrix4f result_pose = Eigen::Matrix4f::Identity();
    Registration(map_cloud_ptr, scan_cloud_ptr, scan_pose, result_pose);

    // 计算相对位姿
    current_loop_pose_.pose = map_pose.inverse() * result_pose;

    // 判断是否有效
    if (registration_ptr_->GetFitnessScore() > fitness_score_limit_)
    {
        LOG(INFO)<<"[Bad Fit]: show the fitscore "<<registration_ptr_->GetFitnessScore();
        return false;
    }
       
    
    static int loop_close_cnt = 0;
    loop_close_cnt ++;

    std::cout << "检测到闭环 "<<  loop_close_cnt
              << ": 帧" << current_loop_pose_.index0 
              << "------>" << "帧" << current_loop_pose_.index1 << std::endl
              << "fitness score: " << registration_ptr_->GetFitnessScore() 
              << std::endl << std::endl;

    // std::cout << "相对位姿 x y z roll pitch yaw:";
    // PrintInfo::PrintPose("", current_loop_pose_.pose);

    return true;
}
bool SCLoopAdapter::JointMap(int key_frame_index, CloudData::CLOUD_PTR& map_cloud_ptr, Eigen::Matrix4f& map_pose) {
    map_pose = all_key_gnss_.at(key_frame_index).pose;
    current_loop_pose_.index0 = all_key_frames_.at(key_frame_index).index;
    
    // 合成地图
    Eigen::Matrix4f pose_to_gnss = map_pose * all_key_frames_.at(key_frame_index).pose.inverse();
    
    for (int i = key_frame_index - extend_frame_num_; i < key_frame_index + extend_frame_num_; ++i) {
        std::string file_path = key_frames_path_ + "/key_frame_" + std::to_string(all_key_frames_.at(i).index) + ".pcd";
        
        CloudData::CLOUD_PTR cloud_ptr(new CloudData::CLOUD());
        pcl::io::loadPCDFile(file_path, *cloud_ptr);
        
        Eigen::Matrix4f cloud_pose = pose_to_gnss * all_key_frames_.at(i).pose;
        pcl::transformPointCloud(*cloud_ptr, *cloud_ptr, cloud_pose);

        *map_cloud_ptr += *cloud_ptr;
    }
    map_filter_ptr_->Filter(map_cloud_ptr, map_cloud_ptr);
    return true;
}

bool SCLoopAdapter::JointScan(CloudData::CLOUD_PTR& scan_cloud_ptr, Eigen::Matrix4f& scan_pose) {
    scan_pose = all_key_gnss_.back().pose;
    current_loop_pose_.index1 = all_key_frames_.back().index;
    current_loop_pose_.time = all_key_frames_.back().time;

    std::string file_path = key_frames_path_ + "/key_frame_" + std::to_string(all_key_frames_.back().index) + ".pcd";
    pcl::io::loadPCDFile(file_path, *scan_cloud_ptr);
    scan_filter_ptr_->Filter(scan_cloud_ptr, scan_cloud_ptr);

    return true;
}

bool SCLoopAdapter::Registration(CloudData::CLOUD_PTR& map_cloud_ptr, 
                               CloudData::CLOUD_PTR& scan_cloud_ptr, 
                               Eigen::Matrix4f& scan_pose, 
                               Eigen::Matrix4f& result_pose) {
    // 点云匹配
    CloudData::CLOUD_PTR result_cloud_ptr(new CloudData::CLOUD());
    registration_ptr_->SetInputTarget(map_cloud_ptr);
    registration_ptr_->ScanMatch(scan_cloud_ptr, scan_pose, result_cloud_ptr, result_pose);

    return true;
}

bool SCLoopAdapter::HasNewLoopPose() {
    return has_new_loop_pose_;
}

LoopPose& SCLoopAdapter::GetCurrentLoopPose() {
    return current_loop_pose_;
}

}


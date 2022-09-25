/*
 * @Author: Liu Weilong
 * @Date: 2020-10-24 10:33:01
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2020-10-25 13:26:06
 * @Description: SCLoop Adapter
 */
#include <string>
#include <deque>
#include <Eigen/Dense>
#include <pcl/registration/ndt.h>
#include <yaml-cpp/yaml.h>
#include <glog/logging.h>

#include "lidar_localization/sensor_data/key_frame.hpp"
#include "lidar_localization/sensor_data/loop_pose.hpp"
#include "lidar_localization/models/registration/registration_interface.hpp"
#include "lidar_localization/models/cloud_filter/cloud_filter_interface.hpp"
#include "lidar_localization/mapping/loop_closing/Scancontext.h"


namespace lidar_localization
{
    class SCLoopAdapter
    {
        public:
        SCLoopAdapter():sc_manager_(){
            LOG(INFO)<<"in SCLoop Adapter ctor";
            InitWithConfig();
        }
        
        bool Update(const KeyFrame key_frame, const KeyFrame key_gnss);
        bool HasNewLoopPose();
        LoopPose& GetCurrentLoopPose();
        
        private:

        bool InitWithConfig();
        bool InitParam(const YAML::Node& config_node);
        bool InitDataPath(const YAML::Node& config_node);
        bool InitRegistration(std::shared_ptr<RegistrationInterface>& registration_ptr, const YAML::Node& config_node);
        bool InitFilter(std::string filter_user, std::shared_ptr<CloudFilterInterface>& filter_ptr, const YAML::Node& config_node);


        bool UpdateSCManager();
        bool DetectNearestKeyFrame(int & index,float& yaw_predict);
        bool CloudRegistration(int index, float yaw_predict);
        bool JointMap(int key_frame_index, CloudData::CLOUD_PTR& map_cloud_ptr, Eigen::Matrix4f& map_pose);
        bool JointScan(CloudData::CLOUD_PTR& scan_cloud_ptr, Eigen::Matrix4f& scan_pose);
        bool Registration(CloudData::CLOUD_PTR& map_cloud_ptr, 
                        CloudData::CLOUD_PTR& scan_cloud_ptr, 
                        Eigen::Matrix4f& scan_pose, 
                        Eigen::Matrix4f& result_pose);
        

        std::deque<KeyFrame> all_key_frames_;
        std::deque<KeyFrame> all_key_gnss_;
        SCManager sc_manager_;

        std::shared_ptr<CloudFilterInterface> scan_filter_ptr_;
        std::shared_ptr<CloudFilterInterface> map_filter_ptr_;
        std::shared_ptr<RegistrationInterface> registration_ptr_; 

        bool has_new_loop_pose_;
        LoopPose current_loop_pose_;
        
        std::string key_frames_path_ = "";
        int extend_frame_num_ = 3;
        int loop_step_ = 10;
        int diff_num_ = 100;
        float detect_area_ = 10.0;
        float fitness_score_limit_ = 2.0;
    };
}
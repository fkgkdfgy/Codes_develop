/*
 * @Author: Liu Weilong
 * @Date: 2021-03-25 18:13:18
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-05-20 19:22:05
 * @FilePath: /3rd-test-learning/38. line_feature/vanishing_point/code/environment_builder.h
 * @Description: 
 */

#include <iostream>
#include <vector>
#include <memory>

#include "Eigen/Eigen"
#include "sophus/se3.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;


namespace calib_sim_builder
{
    class Object
    {
        public:

        Object(const string & frame, const string & parent_frame, bool fixed):
        frame_(frame),parent_frame_(parent_frame),fixed_(fixed)
        {}
        string parent_frame_;
        string frame_;
        bool fixed_;
        //  T_{frame}^{parent_frame}
    };

    class RotationObject:public Object
    {
        public:
        RotationObject(const string & frame, const string & parent_frame, bool fixed,
        Eigen::Quaterniond relative):Object(frame,parent_frame,fixed),relative_rotation(relative)
        {}
        Eigen::Quaterniond relative_rotation;
    };  

    struct IMUGravityInfo
    {
        Eigen::Vector3d gravity_;
        Eigen::Quaterniond self_rotation_; 
    };

    class IMUGravityModel:public RotationObject
    {
        IMUGravityModel(const string & frame, const string & parent_frame, bool fixed,
        Eigen::Quaterniond relative):Object(frame,parent_frame,fixed),relative_rotation(relative)
        {}
        Eigen::Quaterniond relative_rotation;
    };

    class LidarGroundModel:public RotationObject
    {

    }

    
    class Rotation
    {
        public:
        static Rotation create(Eigen::Vector3d axis,double angle,double interval)
        {
            Rotation rot;
            for(double r = 0.0;r<angle;r+=interval)
            {
                Eigen::AngleAxisd aa(r,axis);
                rot.motion_array_.push_back(Eigen::Quaterniond(aa));
            }
            return rot;
        }  
        std::vector<Eigen::Quaterniond> motion_array_; 
    };

    class TrajectoryEngine
    {
        public:
        // 添加坐标系
        void addObject(std::shared_ptr<Object> & obj_ptr){obj_ptr_array_.push_back(obj_ptr);}
        std::vector<std::shared_ptr<Object>>  obj_ptr_array_;
    };

    class RotationTrajectoryEngine:public TrajectoryEngine
    {
        public:

        void checkoutSensorInfo()
        void addRotation(const Rotation & r){rotation_array_.push_back(r);}
        std::vector<Rotation> rotation_array_;
    }

};
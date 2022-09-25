/*
 * @Author: Liu Weilong
 * @Date: 2021-01-06 11:01:19
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-01-13 14:04:10
 * @FilePath: /3rd-test-learning/9. ros-melodic/src/ros-melodic-test/src/9.6 tf_and_message_self_define.cpp
 * @Description: 
 * 
 *               1. 用于进行 ros - melodic - tf 的学习
 *               2. 测试了一下cv::Mat 的内部存储结构是不是一个ColMajor 测试之后发现果然shi
 */
#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <cmath>

#include "opencv2/core/core.hpp"
#include "Eigen/Eigen"
#include "Eigen/Geometry"
#include "sophus/so3.hpp"



#include "ros/ros.h"
#include "tf2/LinearMath/Quaternion.h"
#include "tf2_ros/transform_broadcaster.h"
#include "geometry_msgs/TransformStamped.h"

// #define MSG_COMPILE

#ifdef MSG_COMPILE
#include "devel/include/ros_melodic_test/VisualOdometryMsg.h"
#endif


#define _S_TF_BROADCASTER_ namespace tf_broadcaster{
#define _E_TF_BROADCASTER_ };

_S_TF_BROADCASTER_

class TfBroadcaster
{
    public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    TfBroadcaster(const std::string & frame_id, const std::string & child_frame_id,bool tf = false):
    nh_(),frame_id_(frame_id),child_frame_id_(child_frame_id)
    {
        if(tf)
        {
            std::thread t(&TfBroadcaster::RunTF,this);
            t.detach();
        }
        else
        {
#ifdef MSG_COMPILE
            pose_message_pub_ = nh_.advertise<ros_melodic_test::VisualOdometryMsg>("VisualOdometry",1);
            std::thread t(&TfBroadcaster::RunMessage,this);
            t.detach();
#endif
        }
    }
    // 两者只能开线程跑一个
    // 不要开两个线程同时跑，只有一个锁的情况下，会不会出问题还没有进行过测试
    void RunTF();
#ifdef MSG_COMPILE
    void RunMessage();
#endif
    bool SetTransform(const Eigen::Matrix4d & tf,
                      ros::Time time_stamp = ros::Time::now(),
                      const double translation_weight = 0.0,
                      const double rotation_weight = 0.0);
    //  这里默认 Mat 内部存储方式是 CV_32F
    bool SetTransform(cv::Mat & tf,
                      ros::Time time_stamp = ros::Time::now(),
                      const double translation_weight = 0.0,
                      const double rotation_weight = 0.0);

    bool SetTransform(const Eigen::Vector3d & translation, 
                      const Eigen::Matrix3d & rotation,
                      ros::Time time_stamp = ros::Time::now(),
                      const double translation_weight= 0.0,
                      const double rotation_weight = 0.0);

    bool SetTransform(const Eigen::Vector3d & translation, 
                      const Sophus::SO3d & rotation,
                      ros::Time time_stamp = ros::Time::now(),
                      const double translation_weight = 0.0,
                      const double rotation_weight = 0.0);
                      
    bool SetTransform(const Eigen::Vector3d & translation, 
                      const Eigen::Quaterniond & rotation,
                      ros::Time time_stamp = ros::Time::now(),
                      const double translation_weight = 0.0,
                      const double rotation_weight = 0.0);
    

    

    private:

    bool CheckRotation(const Eigen::Matrix3d & rotation)const ;
    void LoadTransfromStamped(geometry_msgs::TransformStamped & tfs) const;
#ifdef MSG_COMPILE
    void LoadVOMsg(ros_melodic_test::VisualOdometryMsg & vo)const;
#endif
    private:
    
    ros::NodeHandle nh_;
    ros::Publisher  pose_message_pub_;
    ros::Time       cur_time_;

    std::string     frame_id_;
    std::string     child_frame_id_;

    double          cur_translation_weight_=0;
    double          cur_rotation_weight_=0;


    Eigen::Vector3d          translation_;
    Sophus::SO3d             rotation_;
    
    mutable std::mutex       transform_lock_;
    mutable std::atomic_bool pose_update_;
    mutable std::condition_variable pose_condition_variable_;
};


void TfBroadcaster::RunTF()
{
    geometry_msgs::TransformStamped tfs;
    while(ros::ok())
    {
        {
            std::unique_lock<std::mutex> locker(transform_lock_);
            pose_condition_variable_.wait(locker,[this]()->bool{return pose_update_ == true;});
            LoadTransfromStamped(tfs);
            static tf2_ros::TransformBroadcaster br;
            br.sendTransform(tfs);
            pose_update_ = false;
            // std::cout<<"pub a pose"<<std::endl;
        }
        // std::cout<<" TF is Running "<<std::endl;
    }
}
#ifdef MSG_COMPILE
void TfBroadcaster::RunMessage()
{
    ros_melodic_test::VisualOdometryMsg vo_msg;
    while(ros::ok())
    {
        {
            std::unique_lock<std::mutex> locker(transform_lock_);
            pose_condition_variable_.wait(locker,[this]()->bool{return pose_update_ == true;});
            LoadVOMsg(vo_msg);
            pose_message_pub_.publish(vo_msg);


            pose_update_ = false;
            std::cout<<"pub a message"<<std::endl;
        }
        // std::cout<<" TF is Running "<<std::endl;
    }
}
#endif

bool TfBroadcaster::SetTransform(const Eigen::Matrix4d & tf,
                                 ros::Time time_stamp,
                                 const double translation_weight,
                                 const double rotation_weight)
{
    std::lock_guard<std::mutex> locker(transform_lock_);
    Eigen::Matrix3d rotation = tf.block<3,3>(0,0);
    Eigen::Vector3d translation = tf.block<3,1>(0,3);
    if(!CheckRotation(rotation))
    {
        std::cout<<"the input transfrom does not have a rotation matrix !"<<std::endl;
        return false;
    }
    Eigen::AngleAxisd aa(rotation);
    rotation_ = Sophus::SO3d::exp(aa.angle()*aa.axis());
    translation_ = translation;
    cur_time_ = time_stamp;
    cur_translation_weight_ = translation_weight;
    cur_rotation_weight_ = rotation_weight;
    pose_update_ = true;
    pose_condition_variable_.notify_one();
    return true;
}

bool TfBroadcaster::SetTransform(cv::Mat & tf,
                                 ros::Time time_stamp ,
                                 const double translation_weight ,
                                 const double rotation_weight )
{
    Eigen::Matrix4d tf_eigen = Eigen::Matrix4d::Zero();
    tf_eigen << tf.at<float>(0,0), tf.at<float>(0,1), tf.at<float>(0,2),tf.at<float>(0,3),
        tf.at<float>(1,0), tf.at<float>(1,1), tf.at<float>(1,2),tf.at<float>(1,3),
        tf.at<float>(2,0), tf.at<float>(2,1), tf.at<float>(2,2),tf.at<float>(2,3),
        0.0,0.0,0.0,1.0;
    SetTransform(tf_eigen,time_stamp,translation_weight,rotation_weight);
}

bool TfBroadcaster::SetTransform(const Eigen::Vector3d & translation, 
                                 const Eigen::Matrix3d & rotation,
                                 ros::Time time_stamp,
                                 const double translation_weight,
                                 const double rotation_weight)
{
    std::lock_guard<std::mutex> locker(transform_lock_);
    if(!CheckRotation(rotation))
    {
        std::cout<<"the input transfrom does not have a rotation matrix !"<<std::endl;
        return false;
    }
    Eigen::AngleAxisd aa(rotation);
    rotation_ = Sophus::SO3d::exp(aa.angle()*aa.axis());
    translation_ = translation;
    cur_time_ = time_stamp;
    cur_translation_weight_ = translation_weight;
    cur_rotation_weight_ = rotation_weight;
    pose_update_ = true;
    pose_condition_variable_.notify_one();
    return true;
}

bool TfBroadcaster::SetTransform(const Eigen::Vector3d & translation, 
                                 const Sophus::SO3d & rotation,
                                 ros::Time time_stamp,
                                 const double translation_weight,
                                 const double rotation_weight)
{
    std::lock_guard<std::mutex> locker(transform_lock_);
    rotation_ = rotation;
    translation_ = translation;
    cur_time_ = time_stamp;
    cur_translation_weight_ = translation_weight;
    cur_rotation_weight_ = rotation_weight;
    pose_update_ = true;
    pose_condition_variable_.notify_one();
    return true;
}

bool TfBroadcaster::SetTransform(const Eigen::Vector3d & translation, 
                                 const Eigen::Quaterniond & rotation,
                                 ros::Time time_stamp,
                                 const double translation_weight,
                                 const double rotation_weight)
{
    std::lock_guard<std::mutex> locker(transform_lock_);
    Eigen::AngleAxisd aa(rotation);
    rotation_ = Sophus::SO3d::exp(aa.angle()*aa.axis());
    translation_ = translation;
    cur_time_ = time_stamp;
    cur_translation_weight_ = translation_weight;
    cur_rotation_weight_ = rotation_weight;
    pose_update_ = true;
    pose_condition_variable_.notify_one();
    return true;
}

bool TfBroadcaster::CheckRotation(const Eigen::Matrix3d & rotation)const 
{
    double norm = (rotation * rotation.transpose()).determinant();
    if(norm<1.0-1e-3&& norm > 1.0+1e-3)
    return false;
    return true;
}

void TfBroadcaster::LoadTransfromStamped(geometry_msgs::TransformStamped & tfs)const 
{
    tfs.child_frame_id = child_frame_id_;
    tfs.header.stamp = cur_time_;
    tfs.header.frame_id = frame_id_;

    // transfer the data type from SO3 to Quaternion
    Eigen::Quaterniond temp_q  = rotation_.unit_quaternion();
    tfs.transform.rotation.w = temp_q.w();
    tfs.transform.rotation.x = temp_q.x();
    tfs.transform.rotation.y = temp_q.y();
    tfs.transform.rotation.z = temp_q.z();
    
    tfs.transform.translation.x = translation_.x();
    tfs.transform.translation.y = translation_.y();
    tfs.transform.translation.z = translation_.z();
}
#ifdef MSG_COMPILE
void TfBroadcaster::LoadVOMsg(ros_melodic_test::VisualOdometryMsg & vo)const
{   
    vo.header.frame_id = child_frame_id_;
    vo.header.stamp = cur_time_;

    Eigen::Quaterniond temp_q  = rotation_.unit_quaternion();
    vo.cam_in_map_pose.orientation.w = temp_q.w();
    vo.cam_in_map_pose.orientation.x = temp_q.x();
    vo.cam_in_map_pose.orientation.y = temp_q.y();
    vo.cam_in_map_pose.orientation.z = temp_q.z();

    vo.cam_in_map_pose.position.x = translation_.x();
    vo.cam_in_map_pose.position.y = translation_.y();
    vo.cam_in_map_pose.position.z = translation_.z();

    vo.rotation_weight = cur_rotation_weight_;
    vo.translation_weight = cur_translation_weight_;
}
#endif
_E_TF_BROADCASTER_



void FillPose(tf_broadcaster::TfBroadcaster * tfs_ptr)
{
    ros::Rate r(10);
    int count =0;
    // 先走直线
    Eigen::Vector3d translation = Eigen::Vector3d::Zero();
    Eigen::Matrix3d rotation = Eigen::Matrix3d::Identity();
    while(count<50)
    {
        translation += Eigen::Vector3d::Ones()*0.1;
        tfs_ptr->SetTransform(translation,rotation);
        r.sleep();
        count++;
    }
    // 然后进行旋转
    double angle = 0.0;
    while(ros::ok())
    {
        angle += 0.1;
        Eigen::AngleAxisd aa(angle,Eigen::Vector3d(0,0,1));
        tfs_ptr->SetTransform(translation,aa.matrix());
        r.sleep();
    }
}


int main(int argc, char ** argv)
{

    // -90.6805,-0.155628,90.4006,-153,-33,370,0.254107

    Eigen::Vector3d position(-153,-33,370);
    position*=0.001;

    double roll = -90.6805/180.0*M_PI;
    double pitch = -0.155628/180.0*M_PI;
    double yaw = 90.4006/180.0*M_PI;
    Eigen::Matrix3d rotation;
    rotation.setZero();
    rotation =   Eigen::AngleAxisd(yaw, Eigen::Vector3d::UnitZ()).matrix()
               * Eigen::AngleAxisd(pitch, Eigen::Vector3d::UnitY()).matrix()
               * Eigen::AngleAxisd(roll, Eigen::Vector3d::UnitX()).matrix();
    ros::init(argc,argv,"tf_broadcaster_test");

    const std::string frame_id = "map2",child_frame_id="camera";
    tf_broadcaster::TfBroadcaster tf_broadcaster_(frame_id,child_frame_id,1);

    tf_broadcaster_.SetTransform(position,rotation);
    
    std::cout<<"the rotation is "<< std::endl<< rotation<<std::endl;
    std::cout<<"the translation is "<<std::endl<<position.transpose()<<std::endl;
    Eigen::Matrix4d transform;
    transform.setIdentity();
    transform.block<3,3>(0,0) = rotation;
    transform.block<3,1>(0,3) = position;

    cv::Mat transform_mat(4,4,CV_64F);
    transform_mat.data = (unsigned char *)transform.data();
    transform_mat=transform_mat.t();
    std::cout<<"the cv transform is "<< transform_mat<<std::endl;

    std::cout<<"the transform inv is "<< transform_mat.inv()<<std::endl;
    
    ros::Rate r(10);

    while(ros::ok())
    {
        tf_broadcaster_.SetTransform(position,rotation);
        r.sleep();
    }
    return 0;
}
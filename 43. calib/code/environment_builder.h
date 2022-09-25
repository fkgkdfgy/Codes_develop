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

#include "Eigen/Eigen"
#include "sophus/se3.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;


class Object
{
    public:
    Eigen::Quaterniond Camera;
    Eigen::Quaterniond Camera_MW;
    Eigen::Quaterniond Vehicle;
    std::vector<Eigen::Quaterniond> VehicleMotion;
    std::vector<Eigen::Quaterniond> CameraMotion;
    std::vector<Eigen::Quaterniond> MWMotion;
    Object(int i);
};

Object::Object(int i)
{
    Eigen::AngleAxisd R_vehicle_camera(0.3,Eigen::Vector3d(0,0.1,0.9).normalized());
    Eigen::AngleAxisd R_vehicle_MW(-0.4,Eigen::Vector3d(0.1,0.3,1.1).normalized());
    Camera = R_vehicle_camera;
    Camera_MW = R_vehicle_MW;
}


// 这里的旋转是
class Rotation
{
    public:
    static Rotation create(Eigen::Vector3d axis,double angle,double interval)
    {
        Rotation rot;
        for(double r = 0.0;r<angle;r+=interval)
        {
            Eigen::AngleAxisd aa(r,axis);
            rot.MotionArray.push_back(Eigen::Quaterniond(aa));
        }
        return rot;
    }  
    std::vector<Eigen::Quaterniond> MotionArray; 
};

class TrajectoryEngine
{
    public:
    
    void addRotation(const Rotation & rot) {RotArray.push_back(rot);}
    // 运动均为右乘
    void addMotionForObject(Object & obj);
    std::vector<Rotation> RotArray;
};

void TrajectoryEngine::addMotionForObject(Object & obj)
{
    int size =0;
    for(auto & r:RotArray) size+= r.MotionArray.size();

    obj.VehicleMotion.clear();
    obj.VehicleMotion.reserve(size);
    obj.VehicleMotion.push_back(Eigen::Quaterniond(Eigen::Matrix3d::Identity()));

    // 给vehicle 进行赋值
    for(auto & r:RotArray)
    {
        Eigen::Quaterniond label_rot=obj.VehicleMotion.back();
        for(auto & mt:r.MotionArray)
        {
            obj.VehicleMotion.push_back(label_rot * mt);
        }
    }

    obj.CameraMotion.clear();
    obj.CameraMotion.reserve(size);
    obj.CameraMotion.push_back(Eigen::Quaterniond(Eigen::Matrix3d::Identity()));


    obj.MWMotion.clear();
    obj.MWMotion.reserve(size);
    obj.MWMotion.push_back(Eigen::Quaterniond(Eigen::Matrix3d::Identity()));

    for(int i=1;i<size;i++)
    {
        Eigen::Quaterniond relative_rot_v1_v2 = obj.VehicleMotion[i-1].inverse()*obj.VehicleMotion[i];
        Eigen::Quaterniond relative_rot_c1_c2 = obj.Camera.inverse()*relative_rot_v1_v2*obj.Camera;
        Eigen::Quaterniond relative_rot_mw1_mw2 = obj.Camera_MW.inverse()*relative_rot_v1_v2*obj.Camera_MW;

        obj.CameraMotion.push_back(obj.CameraMotion.back()*relative_rot_c1_c2);
        obj.MWMotion.push_back(obj.MWMotion.back()* relative_rot_mw1_mw2);
    }
}


class IMU
{
    public:
    
    Eigen::Vector3d AccBias; 
    Eigen::Vector3d OmegaBias;
    Eigen::Vector3d AccNoise;
    Eigen::Vector3d GyroNoise;
};

class IMUMeasurement
{
    public:

    Eigen::Vector3d Acc;
    Eigen::Vector3d Omega;

    Eigen::Vector3d RealAcc;
    Eigen::Vector3d RealOmega;
};

class IMUState
{
    public:

    IMUState();
    Eigen::Vector3d Position;
    Eigen::Vector3d Velocity;
    Eigen::Vector3d Theta;

    Eigen::Vector3d AccBias;
    Eigen::Vector3d OmegaBias;
    
    Eigen::Matrix3d PositionCov;
    Eigen::Matrix3d VelocityCov;
    Eigen::Matrix3d ThetaCov;
    Eigen::Matrix3d AccBiasCov;
    Eigen::Matrix3d OmegaBiasCov;

    Eigen::Matrix<double,15,15> Jacobian;
};

IMUState::IMUState()
{

}

class IMUPropogator
{
    public:
    
    void prepogate();
    void addInput(const vector<IMUMeasurement> & input){
        imu_measurement = input;
    }
    std::vector<IMUMeasurement> imu_measurement; 
    IMUState imu;
};

void IMUPropogator::prepogate()
{
    // pro
}



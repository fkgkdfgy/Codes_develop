/*
 * @Author: Liu Weilong
 * @Date: 2020-11-14 22:25:00
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2020-11-23 07:42:14
 * @Description: 位姿递推
 *               sample 1. 匀速模型
 *               很奇怪跑
 */

#include "read_csv.h"


#include "Eigen/Eigen"
#include "sophus/so3.hpp"
#include <iostream>
#include <vector>
#include <string>
using namespace std;
using namespace Eigen;

void transformIntoVector(const std::vector<std::vector<double>> & data, std::vector<Eigen::Vector3d> & output)
{
    output.clear();
    output.reserve(data.front().size());
    for(int i=0;i<data.front().size();i++)
    {
        output.push_back(Eigen::Vector3d(data.at(0)[i],data.at(1)[i],data.at(2)[i]));
    }

}
void IntegrationEuler(const vector<Vector3d> & gyro_measurements,
                      double dt,vector<Matrix3d> & output)
{
    Sophus::SO3d cur = Sophus::SO3d::exp(Vector3d::Zero());
    output.reserve(gyro_measurements.size());
    for(auto & measurement: gyro_measurements)
    {
        // 
        cur = cur * Sophus::SO3d::exp(measurement *dt/180.0*M_PI);
        output.push_back(cur.matrix());
    }
}


class PoseExtrapolator
{
    public:
    PoseExtrapolator(std::vector<Eigen::Vector3d> * accel_data_ptr,
                     std::vector<Eigen::Vector3d> * gyro_data_ptr,
                     double hz):
                     accel_data_ptr_(accel_data_ptr),
                     gyro_data_ptr_(gyro_data_ptr),
                     hz_(hz){}

    void buildProblem()
    {
        double deltaT = 1.0/hz_;
        vector<Vector3d> output_angle_xyz;
        vector<Vector3d> vel_output;
        vector<Vector3d> pos_output;
        ExtrapolationWithDeltaAngle(*gyro_data_ptr_,deltaT,output_angle_xyz);
        ExtrapolationForPosition(*accel_data_ptr_,output_angle_xyz,deltaT,vel_output,pos_output);
        cout<<"show the Delta Angle result"<< endl
            << output_angle_xyz.back().transpose()<<endl;
        cout<<"show the Delta Angle result"<< endl
            << Sophus::SO3d::exp(output_angle_xyz.back()).matrix()<<endl;
        
        ExtrapolationWithEquivalentRotationAxis(*gyro_data_ptr_,deltaT,output_angle_xyz);
        ExtrapolationForPosition(*accel_data_ptr_,output_angle_xyz,deltaT,vel_output,pos_output);
        cout<<"show the EquivalentRotationAxis result"<< endl
            << output_angle_xyz.back().transpose()<<endl;
        cout<<"show the EquivalentRotationAxis result"<< endl
            << Sophus::SO3d::exp(output_angle_xyz.back()).matrix()<<endl;
    }

    private:
    // 角增量的方法
    void ExtrapolationWithDeltaAngle(const vector<Vector3d> & gyro_measurements,
                         double dt,vector<Vector3d> & output_angle_xyz)
    {
        Sophus::SO3d cur = Sophus::SO3d::exp(Vector3d::Zero());
        output_angle_xyz.clear();
        output_angle_xyz.reserve(gyro_measurements.size());
        output_angle_xyz.push_back(cur.log());
        for(int i =0;(i+1)<gyro_measurements.size()-1;i=i+2)
        {
            auto deltaA1 = gyro_measurements[i]*dt/180.0*M_PI;
            auto deltaA2 = gyro_measurements[i+1]*dt/180.0*M_PI;
            auto incre_deltaA = deltaA1 + deltaA2;
            cur = cur* Sophus::SO3d::exp(incre_deltaA);
            output_angle_xyz.push_back(cur.log());
            output_angle_xyz.push_back(cur.log());
        }
    }
    
    // 等效旋转矢量的方法
    void ExtrapolationWithEquivalentRotationAxis(const vector<Vector3d> & gyro_measurements,
                                                 double dt,vector<Vector3d> & output_angle_xyz)
    {
        Sophus::SO3d cur = Sophus::SO3d::exp(Vector3d::Zero());
        output_angle_xyz.reserve(gyro_measurements.size());
        auto cur_angle = cur.log();
        output_angle_xyz.push_back(cur_angle);
        for(int i =0;(i+1)<gyro_measurements.size()-1;i=i+2)
        {
            auto deltaA1 = gyro_measurements[i]*dt/180.0*M_PI;
            auto deltaA2 = gyro_measurements[i+1]*dt/180.0*M_PI;
            auto delta_whole =  deltaA1 + deltaA2 + Sophus::SO3d::hat(deltaA1)*deltaA2*2.0/3.0;
            cur = cur* Sophus::SO3d::exp(delta_whole);
            output_angle_xyz.push_back(cur.log());
            output_angle_xyz.push_back(cur.log());
        }
    }
    
    void ExtrapolationForPosition(const std::vector<Vector3d> & accel_measurement,
                                  const std::vector<Vector3d> & angle_axis,
                                  double dt, 
                                  vector<Vector3d> & vel_output,
                                  vector<Vector3d> & pos_output)
    {
        Eigen::Vector3d cur_vel = Eigen::Vector3d::Zero();
        Eigen::Vector3d cur_pos = Eigen::Vector3d::Zero();
        Eigen::Vector3d gravity (0,0,-9.79481);
        vel_output.clear();
        pos_output.clear();
        vel_output.reserve(angle_axis.size());
        pos_output.reserve(angle_axis.size());
        for(int i=0 ;i<angle_axis.size();i++)
        {
            if(i==0)
            continue;
            Eigen::AngleAxisd prev_rotation(angle_axis.at(i-1).norm(),angle_axis.at(i-1).normalized());
            Eigen::AngleAxisd cur_rotation(angle_axis.at(i).norm(),angle_axis.at(i).normalized());
            cur_vel = cur_vel + dt*(0.5*prev_rotation.matrix()*accel_measurement[i-1]+
                                    0.5*cur_rotation.matrix()*accel_measurement[i] - gravity);
            vel_output.push_back(cur_vel);
            cur_pos = cur_pos + 0.5*(vel_output[i]+ vel_output[i-1]);
            pos_output.push_back(cur_pos);
        }
    }

    private:
    std::vector<Eigen::Vector3d> * accel_data_ptr_;
    std::vector<Eigen::Vector3d> * gyro_data_ptr_;
    double hz_;
};

int main()
{
    // =================================== 定轴测试 ======================================
    std::string gyro_real_measurement = "/home/lwl/workspace/HW/gnss-ins-sim/demo_motion_def_files/imu_simulation_hw1/ref_gyro.csv";
    std::string gyro_measurement = "/home/lwl/workspace/HW/gnss-ins-sim/demo_motion_def_files/imu_simulation_hw1/gyro-0.csv";
    std::string gravity_measurement = "/home/lwl/workspace/HW/gnss-ins-sim/demo_motion_def_files/imu_simulation_hw1/ref_accel.csv";
    std::string accel_measurement = "/home/lwl/workspace/HW/gnss-ins-sim/demo_motion_def_files/imu_simulation_hw1/accel-0.csv";
    std::vector<std::string> headers_real;
    std::vector<std::vector<double>> data_real;
    std::vector<std::string> headers;
    std::vector<std::vector<double>> data;
    std::vector<std::string> headers_gravity;
    std::vector<std::vector<double>> data_gravity;
    std::vector<std::string> headers_accel;
    std::vector<std::vector<double>> accel_data;

    std::vector<Eigen::Vector3d> gyro_in_vector;
    std::vector<Eigen::Vector3d> gyro_real_in_vector;
    std::vector<Eigen::Vector3d> accel_in_vector;
    std::vector<Eigen::Vector3d> gravity_in_vector;
    
    if(!readCSV(headers_real,data_real,gyro_real_measurement))
    {
        std::abort();
    };
    if(!readCSV(headers,data,gyro_measurement))
    {
        std::abort();
    };
    if(!readCSV(headers_gravity,data_gravity,gravity_measurement))
    {
        std::abort();
    };
    if(!readCSV(headers_accel,accel_data,accel_measurement))
    {
        std::abort();
    };

    transformIntoVector(data_real,gyro_real_in_vector);
    transformIntoVector(data,gyro_in_vector);
    transformIntoVector(data_gravity,gravity_in_vector);
    transformIntoVector(accel_data,accel_in_vector);


    double hz = 30.0;

    PoseExtrapolator tmp(&accel_in_vector,&gyro_real_in_vector,hz);

    tmp.buildProblem();

    Eigen::Quaterniond tmp_q(8.376852109105588085e-01,3.161446979674942814e-01,4.401753629742948815e-01,6.768801370421564934e-02);
    Eigen::AngleAxisd dream_result(tmp_q);
    cout<<"the dream result is "<<endl
        << dream_result.angle()*dream_result.axis()<<endl
        <<"the dream matrix is "<<endl
        << dream_result.matrix()<<endl;
    std::vector<Matrix3d> result_1,result_2,result_3,result_4;
    IntegrationEuler(gyro_real_in_vector,1.0/hz,result_1);
    cout <<"the estimated result is "<<endl
        << result_1.back()<<endl;
}
        

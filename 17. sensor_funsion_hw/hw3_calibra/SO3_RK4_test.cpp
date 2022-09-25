/*
 * @Author: Liu Weilong
 * @Date: 2020-11-12 10:40:57
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-05-23 19:25:36
 * @FilePath: /3rd-test-learning/19. sensor_funsion_hw/hw3_calibra/SO3_RK4_test.cpp
 * @Description: 用于验证SO3 RK4 的正确性
 *               IntegrationUsedInSemiCalib 积分太过暴力 结果很不好
 * 
 *               坑位预订：1. AngleAxis 的angle 单位是 radian 不能用deg
 *                       2. Sophus::SO3d::exp(so3) so3 的单位是 radian/s 不能直接使用deg/s
 *                       3. 300Hz 
 */
#include "read_csv.h"
#include "sophus/so3.hpp"
#include "Eigen/Eigen"
#include <iostream>
#include <vector>
#include <string>
#include <cmath>

using namespace std;
using namespace Eigen;
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

void IntegrationEulerWithNumeric1stApproximate(const vector<Vector3d> & gyro_measurements,
                                               double dt,vector<Matrix3d> & output)
{
    Sophus::SO3d cur = Sophus::SO3d::exp(Vector3d::Zero());
    auto cur_matrix = cur.matrix();
    output.reserve(gyro_measurements.size());
    for(auto & measurement: gyro_measurements)
    {
        cur_matrix = cur_matrix*(Matrix3d::Identity()+ Sophus::SO3d::hat(measurement/180.0*M_PI)*dt);

        output.push_back(cur_matrix);
    }
}

void IntegrationMidValue(const vector<Vector3d> & gyro_measurements,
                         double dt,vector<Matrix3d> & output)
{
    Sophus::SO3d cur = Sophus::SO3d::exp(Vector3d::Zero());
    output.reserve(gyro_measurements.size());

    // TODO 看一下为什么会错
    // for(auto s_ptr = gyro_measurements.begin(),n_ptr = next(s_ptr);n_ptr!=gyro_measurements.end();next(s_ptr))
    // {
    //     auto midvalue = 0.5*(*s_ptr + * n_ptr);
    //     cur = cur * Sophus::SO3d::exp(midvalue *dt);
    //     output.push_back(cur.matrix());
    //     s_ptr++;
    // }
    for(int i =0;i<gyro_measurements.size()-1;i++)
    {
        auto midvalue = 0.5*(gyro_measurements[i] + gyro_measurements[i+1]);
        cur = cur * Sophus::SO3d::exp(midvalue *dt/180.0*M_PI);
        output.push_back(cur.matrix());
    }
}

// void IntegrationRK4(const vector<Vector3d> & gyro_measurements,
//                     double dt,vector<Matrix3d> & output)
// {
//     Sophus::SO3d cur = Sophus::SO3d::exp(Vector3d::Zero());
//     output.reserve(gyro_measurements.size());
//     for(int i =0;i<gyro_measurements.size()-1;i++)
//     {
//         auto & s = gyro_measurements[i];
//         auto & n = gyro_measurements[i+1];
//         auto tmp_so3 = cur;
//         auto k1 = tmp_so3.matrix()*Sophus::SO3d::hat(s);
//         tmp_so3 = tmp_so3 * Sophus::SO3d::exp(dt*(s));
//         auto k2 = tmp_so3.matrix()*Sophus::SO3d::hat((s+n)*0.5);
//         tmp_so3 = tmp_so3 * Sophus::SO3d::exp((s+n)*0.5*dt);
//         auto k3 = tmp_so3.matrix()*Sophus::SO3d::hat((s+n)*0.5);
//         tmp_so3 = tmp_so3 * Sophus::SO3d::exp((s+n)*0.5*dt);
//         auto k4 = tmp_so3.matrix()*Sophus::SO3d::hat((n)*0.5);
//         cur = cur * Sophus::SO3d::exp(dt*k1/6.0)*
//                     Sophus::SO3d::exp(dt*k2/3.0)*
//                     Sophus::SO3d::exp(dt*k3/3.0)*
//                     Sophus::SO3d::exp(dt*k4/6.0);
//         output.push_back(cur.matrix());
//     }
// }



void IntegrationUsedInSemiCalib(const vector<Vector3d> & gyro_measurements,
                         double dt,vector<Matrix3d> & output)
{
    Sophus::SO3d cur = Sophus::SO3d::exp(Vector3d::Zero());
    Eigen::Matrix3d cur_matrix = cur.matrix();
    output.clear();
    Eigen::Vector3d sum = Eigen::Vector3d::Zero();
    for(auto & measurement:gyro_measurements)
    {
        sum += (measurement/180.0*M_PI);
        cur_matrix = (Eigen::Matrix3d::Identity()+Sophus::SO3d::hat(sum)*dt);
        output.push_back(cur_matrix);
    }
}


int main()
{
    std::string gyro_real_measurement = "/home/lwl/workspace/gnss-ins-sim/demo_saved_data/2021-05-22-20-31-54/ref_gyro.csv";
    std::string gravity_measurement = "/home/lwl/workspace/gnss-ins-sim/demo_saved_data/2021-05-22-20-31-54/ref_accel.csv";
    std::vector<std::string> headers;
    std::vector<std::vector<double>> data;
    std::vector<Vector3d> data_in_vector;

    std::vector<std::string> gravity_headers;
    std::vector<std::vector<double>> gravity_data;
    std::vector<Vector3d> gravity_in_vector;
    
    if(!readCSV(headers,data,gyro_real_measurement))
    {
        std::abort();
    };
    
    if(!readCSV(gravity_headers,gravity_data,gravity_measurement))
    {
        std::abort();
    };

    data_in_vector.reserve(data.front().size());
    
    for(int i=0;i<data.front().size();i++)
    {
        data_in_vector.push_back(Eigen::Vector3d(data.at(0)[i],data.at(1)[i],data.at(2)[i]));
    }

    gravity_in_vector.reserve(gravity_data.front().size());
    
    for(int i=0;i<gravity_data.front().size();i++)
    {
        gravity_in_vector.push_back(Eigen::Vector3d(gravity_data.at(0)[i],gravity_data.at(1)[i],gravity_data.at(2)[i]));
    }

    double deltaT= 1.0/100.0;
    std::vector<Matrix3d> result_1,result_2,result_3,result_4;
    IntegrationEuler(data_in_vector,deltaT,result_1);

    IntegrationEulerWithNumeric1stApproximate(data_in_vector,deltaT,result_2);
    IntegrationMidValue(data_in_vector,deltaT,result_3);
    IntegrationUsedInSemiCalib(data_in_vector,deltaT,result_4);
    // IntegrationRK4(data_in_vector,0.01,result_4);

    // show the final result
    Eigen::Matrix3d rotation_matrix1;
    rotation_matrix1 = Eigen::AngleAxisd(-2.758876806/180.0*M_PI, Eigen::Vector3d::UnitZ()) *
                Eigen::AngleAxisd(-12.07555/180.0*M_PI, Eigen::Vector3d::UnitY()) *
                Eigen::AngleAxisd(-5.24321/180.0*M_PI, Eigen::Vector3d::UnitX());
    cout<<"the result should be "<<endl
        << Sophus::SO3d(rotation_matrix1).log().transpose()<<endl;
    cout<<"the IntegrationEuler result is "<<endl
        << Sophus::SO3d(result_1.back()).log().transpose()<<endl;
    cout<<"the IntegrationEulerWithNumeric1stApproximate result is "<<endl
        << Sophus::SO3d(Eigen::AngleAxisd(result_2.back()).matrix()).log().transpose()<<endl;
    cout<<"the IntegrationMidValue result is "<<endl
        << Sophus::SO3d(Eigen::AngleAxisd(result_3.back()).matrix()).log()<<endl;
    cout<<"the IntegrationUsedInSemiCalib result is "<<endl
        << result_4.back()<<endl;
    // cout<<"the IntegrationRK4 result is "<<endl
    //     << result_4.back()<<endl;
    cout<<"the origin gravity vector is "<<endl
        <<gravity_in_vector.front().transpose()<<endl;
    cout<<"the final gravity is "<<endl
        <<gravity_in_vector.back().transpose()<<endl;
    
    cout<<"the transformed final gravity is "<<endl
        <<rotation_matrix1* gravity_in_vector.back()<<endl;
}
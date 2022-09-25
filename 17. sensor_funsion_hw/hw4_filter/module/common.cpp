/*
 * @Author: Liu Weilong
 * @Date: 2020-11-26 19:43:14
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2020-12-06 14:55:08
 * @Description: 
 */
#include "common.h"

bool AddNoise(std::vector<Eigen::Vector3d> & point_cloud)
{
    // scale 再大基本就挂了  因为点在设置的时候就比较密集
    double scale = 0.01;
    Eigen::Matrix3d covar = Eigen::Matrix3d::Identity()*scale;
    g2o::GaussianSampler<Eigen::Matrix<double,3,1>,Eigen::Matrix<double,3,3>> sampler;
    sampler.setDistribution(covar);
    for(auto & point :point_cloud)
    {
        Eigen::Vector3d noise;
        noise<<sampler.generateSample();
        // cout<<"the noise is "<<noise.transpose()<<endl;
        point = point + noise;
    }
}

bool AddNoiseSO3(std::vector<Eigen::Vector3d> & point_cloud)
{
    // scale 再大基本就挂了  因为点在设置的时候就比较密集
    double scale = 0.001;
    Eigen::Matrix3d covar = Eigen::Matrix3d::Identity()*scale;
    g2o::GaussianSampler<Eigen::Matrix<double,3,1>,Eigen::Matrix<double,3,3>> sampler;
    sampler.setDistribution(covar);
    for(auto & point :point_cloud)
    {
        Eigen::Vector3d noise;
        noise<<sampler.generateSample();
        point = (Sophus::SO3d::exp(point)* Sophus::SO3d::exp(noise)).log();
    }
}



void prepareLaserSimData(const std::vector<Eigen::Vector3d> & real_rotation_data,
                         const std::vector<Eigen::Vector3d> & real_translation_data,
                         std::vector<Eigen::Vector3d> & noise_rotation_data,
                         std::vector<Eigen::Vector3d> & noise_translation_data,
                         double IMU_hz , double measure_hz)
{ 
    // 尽量使用 可以整除 的 IMU_hz 和 measure_hz
    
    int gap = IMU_hz/measure_hz;
    int size_translation = static_cast<double>(real_translation_data.size())/
                           static_cast<double>(gap)+3;
    int size_rotation = size_translation;
    noise_rotation_data.reserve(size_translation);
    noise_translation_data.reserve(size_rotation);
    auto pre_position = real_translation_data.front();
    auto pre_rotation = real_rotation_data.front();
    
    Eigen::Matrix4d T_i_w = Eigen::Matrix4d::Identity();
    T_i_w.block(0,0,3,3) = Sophus::SO3d::exp(pre_rotation).matrix();
    T_i_w.block(0,3,3,1) = pre_position;
    Eigen::Matrix4d T_j_w = Eigen::Matrix4d::Identity();
    for(int j= gap-1;j<real_rotation_data.size();j=j+gap)
    {
        T_j_w.block(0,0,3,3) = Sophus::SO3d::exp(real_rotation_data.at(j)).matrix();
        T_j_w.block(0,3,3,1) = real_translation_data.at(j);
        auto T_j_i = T_i_w.inverse()*T_j_w;
        Eigen::Matrix3d rotation_matrix =T_j_i.block(0,0,3,3); 
        Eigen::AngleAxisd tmp_angleaxis(rotation_matrix);
        noise_rotation_data.push_back(Eigen::Vector3d(T_j_i.block(0,3,3,1)));
        T_i_w = T_j_w;   
    }
    AddNoise(noise_translation_data);
    AddNoiseSO3(noise_rotation_data);
}


void transformIntoVector(const std::vector<std::vector<double>> & data,
                         std::vector<double> & output)
{
    if(data.size()!=0)
    {
        std::cerr<<"[ERROR]: in transformIntoVector there is something wrong with the input data!"<<std::endl;
        std::abort();
    }
    std::copy(data.front().begin(),data.front().end(),output.begin());
}
/*
 * @Author: Liu Weilong
 * @Date: 2021-01-18 14:42:19
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-01-18 15:33:40
 * @FilePath: /3rd-test-learning/22. slam_demo/ground_constraint/path_creator.h
 * @Description: 
 */

#include <iostream>
#include <cmath>
#include <random>

#include "Eigen/Eigen"
#include "sophus/so3.hpp"

#include "ground_constraint_builder.h"

class PathCreator
{
    public:
    
    std::vector<SO3R3> GeneratorPathLine();
    std::vector<SO3R3> AddNoise(const std::vector<SO3R3> & real_data);

};


std::vector<SO3R3> PathCreator::GeneratorPathLine()
{
    std::vector<SO3R3> path;
    path.reserve(2000);
    SO3R3 start,end;
    start._r3 = Eigen::Vector3d(0.0,0.0,0.0);
    end._r3 = Eigen::Vector3d(10.0,10.0,0.0);
    start._SO3 = Sophus::SO3d::exp(Eigen::Vector3d(0.0,0.0,1.0)*M_PI_4);
    end._SO3 = start._SO3;
    double stride = 10.0/1000.0;
    for(int i = 0;i<1000;i++)
    {
        SO3R3 inter;
        inter._r3= Eigen::Vector3d(i*stride,i*stride,0.0);
        inter._SO3 = start._SO3;
        path.push_back(inter);
    }
}
std::vector<SO3R3> AddNoise(const std::vector<SO3R3> & real_data)
{
    // 准备3e4个随机数来进行随机游走
    std::random_device rd; 
    std::mt19937 gen(rd());

    std::vector<double> noise_array;
    std::vector<double> x_noise;
    std::vector<double> y_noise;
    noise_array.reserve(3e4);
    x_noise.reserve(2e3);
    y_noise.reserve(2e3);
    double temp = 0.0;
    std::normal_distribution<> translation_random(0,sqrt(0.0333));
    for(int i=0;i<3e4;i++)
    {
        temp += translation_random(gen);
        noise_array.push_back(temp);
    }

    std::normal_distribution<> rotation_random(0,0.1);
    for(int i=0;i<2e3;i++)
    {
        x_noise.push_back(rotation_random(gen));
        y_noise.push_back(rotation_random(gen));
    }
    
    double max=0.0;
    for(auto e : noise_array)
        if(abs(e)>max)
            max = abs(e);
    
// 让我先写一个优化试一下
            
    
    


    
}

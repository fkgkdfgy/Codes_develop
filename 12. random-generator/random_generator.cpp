/*
 * @Author: Liu Weilong
 * @Date: 2020-10-10 08:51:23
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-03-01 10:57:59
 * @FilePath: /3rd-test-learning/12. random-generator/random_generator.cpp
 * @Description: 用于生成各种随机数
 */

#include <iostream>
#include <random>

using namespace std;

int main(int argc,char **argv)
{
    // 定义随机Engine
    std::random_device rd; 
    std::mt19937 gen(rd());

    // 定义生产器 整数生成器
    std::uniform_int_distribution<> int_random(0,10000);
    int random_1 = int_random(gen);
    cout<<"Generated from the int random engine of uniform :" <<random_1<<endl;

    // 定义生成器 实数生成器
    std::uniform_real_distribution<> real_random(0,10000);
    double random_2 = real_random(gen);
    cout<<"Generated from the real random engine of uniform :" <<random_2<<endl;

    // 定义生成器 高斯分布
    std::normal_distribution<> gaussian_random(0,50);
    double random_3 = gaussian_random(gen);
    cout<<"Generated from the real random engine of gaussian :" <<random_3<<endl;

    return 0;
}
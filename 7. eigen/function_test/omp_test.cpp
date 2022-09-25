/*
 * @Author: Liu Weilong
 * @Date: 2020-11-09 19:20:19
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2020-11-09 20:06:18
 * @Description:  测试omp 能不能在 Eigen之中进行使用
 */

#include "omp.h"
#include "Eigen/Eigen"
#include <chrono>
#include <iostream>
using namespace std;

int main()
{
    Eigen::Vector3d one;
    one<<1.0,1.0,1.0;
    std::vector<Eigen::Vector3d> tmp_array;
    tmp_array.reserve(1000);

    for(int i=0;i<1000;i++)
    {
        Eigen::Vector3d point = one * 0.1; 
        tmp_array.push_back(point);
    }
    
    omp_set_num_threads(5);
    Eigen::Vector3d data_sum = Eigen::Vector3d::Zero();
    auto start = std::chrono::system_clock::now();


    // 测试结果就是并不支持 Eigen
    double x=0.0,y=0.0,z=0.0;
    #pragma omp parallel for reduction(+:x,y,z)
    for(int i =0;i<tmp_array.size();i++ ) 
    {
        x += tmp_array[i].x() * 0.5;
        y += tmp_array[i].y() * 0.5;
        z += tmp_array[i].z() * 0.5;
    }
    auto duration = std::chrono::system_clock::now() - start;

    std::cout<<"the result x :"<< x<<endl
             <<"the result y :"<< y<<endl
             <<"the result z :"<< z<<endl;


    std::cout<<" the comsumed time is " 
             << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count()
             <<"milliseconds"<<std::endl;
    

    
    return 0;
}
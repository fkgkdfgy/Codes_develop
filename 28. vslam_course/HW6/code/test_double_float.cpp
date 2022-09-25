/*
 * @Author: Liu Weilong
 * @Date: 2021-03-02 14:23:08
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-03-02 14:39:08
 * @FilePath: /3rd-test-learning/28. vslam_course/HW6/code/test_double_float.cpp
 * @Description: 
 * 确定 double 和 float 哪个计算更快
 * 结果 float 计算速度大概是 30% 
 */
#include <iostream>
#include <chrono>

using namespace std;
int main()
{
    auto start = chrono::high_resolution_clock::now();
    double a_d=0.;
    float a_f=0.;
    for(int i=0;i<1e10;i++)
    for(int j=0;j<1e10;j++)
    {
        a_d +=0.0004;
        a_d /=0.1;
        a_d *=0.1;
    }
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> diff = end - start;
    cout<<"Total: "<< diff.count()<<" s"<<endl;

     start = chrono::high_resolution_clock::now();
    for(int i=0;i<1e10;i++)
    for(int j=0;j<1e10;j++)
    {
        a_f +=0.0004;
        a_f /=0.1;
        a_f *=0.1;
    }
     end = chrono::high_resolution_clock::now();
     diff = end - start;
    cout<<"Total: "<< diff.count()<<" s"<<endl;
}
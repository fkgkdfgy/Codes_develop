/*
 * @Author: Liu Weilong
 * @Date: 2020-10-17 10:31:52
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2020-10-26 14:46:48
 * @Description: 使用 omp 对积分进行计算
 * 
 *               API 使用：
 *               omp_set_num_threads() // 设置omp 区域内线程总数    
 *               omp_get_num_threads() // 得到omp 区域内线程总数
 *               omp_get_thread_num() // 得到当前线程编号
 */

#include "omp.h"
#include <cmath>
#include <iostream>

double A[1000];
double result[4];
void integratePi()
{
    omp_set_num_threads(4);
    #pragma omp parallel
    {      
        int thread_num = omp_get_thread_num();
        int num_thread = omp_get_num_threads();
        int lower_bound = std::floor(1000/num_thread*thread_num);
        int upper_bound = std::ceil(1000/num_thread*(thread_num+1));
        result[thread_num] = 0;
        for(int i = lower_bound;i < upper_bound;i++)
        {
            result[thread_num] += (4/(1+A[i]*A[i]) * 1/1000);
        }
    }

    double pi = 0;
    for(int i =0;i<4;i++)
    {
        pi += result[i];
    }
    std::cout<<"pi is "<<pi <<std::endl;
}

int main()
{
    for(int i =0;i<1000;i++)
    {
        A[i] = 1.0/1000.0 *float(i+1); // 一定要注意 整数 计算的截断 问题
    }
    
    integratePi();
}


/*
 * @Author: Liu Weilong
 * @Date: 2020-10-22 16:43:30
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2020-10-23 09:34:56
 * @FilePath: /3rd-test-learning/17. openmp/omp_exercise3.cpp
 * @Description: OMP exercise 3 主要是 omp critical 和 omp atomic的使用
 *               
 *               使用API
 *               #pragma omp critical  
 *               #pragma omp atomic      
 * 
 *               critical 和 atomic 的区别
 *               
 *               critical：  
 *               1. 可以保证一整个code block 的独占性
 *               2. 慢
 *               
 *               atomic：
 *               1. 只能用于一些特殊操作 一个语句
 *               2. 快
 *               3. 只支持 simple type (scalar type)
 * 
 */
#include "omp.h"
#include <chrono>
#include <cmath>
#include <iostream>

const unsigned int step_num = 1000;
double A[step_num];
double result = 0;
void integratePi_critical()
{  
    omp_set_num_threads(4);
    #pragma omp parallel
    {      
        int thread_num = omp_get_thread_num();
        int num_thread = omp_get_num_threads();
        int lower_bound = std::floor(step_num/num_thread*thread_num);
        int upper_bound = std::ceil(step_num/num_thread*(thread_num+1));       
            for(int i = lower_bound;i < upper_bound;i++)
            {
                #pragma omp critical 
                result += (4/(1+A[i]*A[i]) * 1/step_num);
            }
    }

    double pi = result;
    std::cout<<"pi is "<<pi <<std::endl;
}

void integratePi_atomic()
{  
    omp_set_num_threads(4);
    result =0;
    #pragma omp parallel
    {      
        int thread_num = omp_get_thread_num();
        int num_thread = omp_get_num_threads();
        int lower_bound = std::floor(step_num/num_thread*thread_num);
        int upper_bound = std::ceil(step_num/num_thread*(thread_num+1));
        
        for(int i = lower_bound;i < upper_bound;i++)
        {
            auto tmp = (4/(1+A[i]*A[i]) * 1/step_num);
            #pragma omp atomic 
            result = tmp+result;
        }
    }

    double pi = result;
    std::cout<<"pi is "<<pi <<std::endl;
}

int main()
{
    for(int i =0;i<step_num;i++)
    {
        A[i] = 1.0/float(step_num) *float(i+1); // 一定要注意 整数 计算的截断 问题
    }
    auto start = std::chrono::steady_clock::now();
    integratePi_critical();
    auto diff = std::chrono::steady_clock::now() - start;
    std::cout<<" the comsumed time is " 
             << std::chrono::duration_cast<std::chrono::microseconds>(diff).count()
             <<"milliseconds"<<std::endl;

    start = std::chrono::steady_clock::now();
    integratePi_atomic();
    diff = std::chrono::steady_clock::now() - start;
    std::cout<<" the comsumed time is " 
             << std::chrono::duration_cast<std::chrono::microseconds>(diff).count()
             <<"milliseconds"<<std::endl;
}

/*
 * @Author: Liu Weilong
 * @Date: 2020-10-26 14:42:50
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2020-10-26 14:45:35
 * @FilePath: /3rd-test-learning/17. openmp/omp_exercise6.cpp
 * @Description:  用于学习Openmp的schedule 方式
 *                dynamic 的方式不暂时进行使用
 */
#include <iostream>
#include "omp.h"
using namespace std;

void print()
{
    omp_set_num_threads(4);
    #pragma omp parallel for schedule(static,1)
    for (int i = 0; i < 16; ++i) {
        cout<<i<<endl;;
    }
} 


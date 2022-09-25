/*
 * @Author: Liu Weilong
 * @Date: 2020-10-17 10:26:02
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2020-10-17 10:33:31
 * @Description: Exercise 1: Hello world!
 *               主要是测试环境
 */
#include "omp.h"
#include <iostream>
using namespace std;
void print()
{
    #pragma omp parallel // 这句话主要是为了标明以下是omp 的多线程区域
    {
        int id = omp_get_thread_num();
        cout<<"hello "<<id<<endl;
        cout<<"world "<<id<<endl;
    }
}

int main()
{
    print();
}
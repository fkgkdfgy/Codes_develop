/*
 * @Author: Liu Weilong
 * @Date: 2020-10-23 09:23:05
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2020-10-26 14:39:42
 * @FilePath: /3rd-test-learning/17. openmp/omp_exercise5.cpp
 * @Description: 用于测试 barrier 和 nowait
 * 
 *               API 测试
 *               #pragma omp barrier 等到所有线程执行到这里，再往下进行
 *               #pragma omp for nowait 打破 omp for 的implicit barrier 让代码继续向后执行
 *               
 *               #pragma omp parallel for 默认barrier  不等待的话需要自行添加nowait
 *               #pragma critical atomic 没有默认 barrier 也就是说就是nowait 的方式进行执行
 */

#include "omp.h"
#include <string>
#include <iostream>
using namespace std;

void print(string str)
{
    cout<<str<<endl;
    cout<<"============================================"<<endl;
    for(int  i=0; i <30;i++)
    {
        cout<<i<<endl;
    }
}

void testBarrierAndNowaits()
{
    omp_set_num_threads(4);
    #pragma omp parallel
    {
        print("test barrier");
        cout<<"mission complete!"<<endl;
        #pragma omp barrier
        #pragma omp for nowait
        for (int i= 0;i<100;i++)
        {
            cout<<i+100<<endl;
        } // 这里如果没有nowait 实际上是有一个隐式的 barrier的

        cout<<" the last time mission complete!"<<endl;
    }
}

int main()
{
    testBarrierAndNowaits();
}
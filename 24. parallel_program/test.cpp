/*
 * @Author: Liu Weilong
 * @Date: 2020-11-16 18:50:55
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2020-11-16 19:04:59
 * @Description: 内存的对齐 计算超级快 差了近4倍
 */
#include <iostream>
#include <chrono>
using namespace std;

int main()
{
    int a[16];
    int b[16];
    for(int i =0;i<16;i++)
    {
        a[i] = i;
        b[i] = i;
    }

    int c[16]{};
    
    auto start = std::chrono::high_resolution_clock::now();

    for(int j=0;j<10000000;j++)
    {
        for(int i=0;i<16;i=i+4)
        {
            c[i] = a[i]*b[i];
            c[i+1] = a[i+1]*b[i+1];
            c[i+2] = a[i+2]*b[i+2];
            c[i+3] = a[i+3]*b[i+3];
        }
    }

    chrono::duration<double> diff = std::chrono::high_resolution_clock::now()-start;
    std::cout<<"duration is "<< std::chrono::duration_cast<std::chrono::milliseconds>(diff).count()<<std::endl;

    for(int j=0;j<10000000;j++)
    {
        for(int i=0;i<16;i=i+8)
        {
            c[i] = a[i]*b[i];
            c[i+1] = a[i+1]*b[i+1];
            c[i+2] = a[i+2]*b[i+2];
            c[i+3] = a[i+3]*b[i+3];
            c[i+4] = a[i+4]*b[i+4];
            c[i+5] = a[i+5]*b[i+5];
            c[i+6] = a[i+6]*b[i+6];
            c[i+7] = a[i+7]*b[i+7];
        }
    }

    diff = std::chrono::high_resolution_clock::now()-start;
    std::cout<<"duration is "<< std::chrono::duration_cast<std::chrono::milliseconds>(diff).count()<<std::endl;



    for(int j=0;j<10000000;j++)
    {
        for(int i=0;i<16;i=i+1)
        {
            c[i] = a[i]*b[i];
        }
    }

    diff = std::chrono::high_resolution_clock::now()-start;
    std::cout<<"duration is "<< std::chrono::duration_cast<std::chrono::milliseconds>(diff).count()<<std::endl;
}
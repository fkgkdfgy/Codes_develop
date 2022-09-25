/*
 * @Author: Liu Weilong
 * @Date: 2020-11-26 20:17:48
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2020-11-26 20:25:29
 * @Description: 主要是用于测试  omp 中的section
 *               使用注意事项：
 *               1. 想要内部进行并行。那么
 *                  必须外面是
 * 
 *                  #pragma omp parallel sections
 * 
 *                  而不是
 * 
 *                  #pragma omp sections
 *                  
 */

#include "omp.h"
#include <iostream>
using namespace std;
int main()
{
    omp_set_num_threads(2);

    #pragma omp parallel sections
    {
        #pragma omp section
        {
            cout<<" the current thread is "<< omp_get_thread_num()<<endl;
        }
        #pragma omp section
        {
            cout<<" the current thread is "<< omp_get_thread_num()<<endl;
        }
                #pragma omp section
        {
            cout<<" the current thread is "<< omp_get_thread_num()<<endl;
        }
                #pragma omp section
        {
            cout<<" the current thread is "<< omp_get_thread_num()<<endl;
        }
                #pragma omp section
        {
            cout<<" the current thread is "<< omp_get_thread_num()<<endl;
        }
    }

    cout<<"==========================================="<<endl;
    
    #pragma omp  sections
    {
        #pragma omp section
        {
            cout<<" the current thread is "<< omp_get_thread_num()<<endl;
        }
        #pragma omp section
        {
            cout<<" the current thread is "<< omp_get_thread_num()<<endl;
        }
                #pragma omp section
        {
            cout<<" the current thread is "<< omp_get_thread_num()<<endl;
        }
                #pragma omp section
        {
            cout<<" the current thread is "<< omp_get_thread_num()<<endl;
        }
                #pragma omp section
        {
            cout<<" the current thread is "<< omp_get_thread_num()<<endl;
        }
    }
    
    return 0;
}
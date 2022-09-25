/*
 * @Author: Liu Weilong
 * @Date: 2020-10-22 18:37:37
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2020-10-23 09:36:26
 * @FilePath: /3rd-test-learning/17. openmp/omp_exercise4_private_shared.cpp
 * @Description: 学习一下 private 和 shared 的使用
 * 
 *               目前来说，和自己的想法还有差距。
 *               
 *               使用API：
 *               
 *               #pragma omp parallel + shared/private
 */

#include "omp.h"
#include <iostream>
#include <cmath>
using namespace std;

void testShareAndPrivate()
{
    int a =10;
    int b =0;
    int c =0;
    omp_set_num_threads(4);
    
    // TODO 以下两个问题完全搞不懂
    // 这里的 b 会被共享 而且似乎是atomic 因为加不加omp atomic 结果都不会乱
    // 还有就是 a 和 被 a 赋值的量一直都是a 进入时候的值                   
    #pragma omp parallel private(a) shared(b)
    {
        int d =a+10;
        cout<< " the update a is "<<d<<endl;
        #pragma omp atomic // 有没有这句话 b 都不会被改变
        b = b+10;
        cout<< "the update b is "<<b<<endl;
        c=c+2324;
        cout<< "the update c is "<<c<<endl;
    }
}

int main()
{
    testShareAndPrivate();
}
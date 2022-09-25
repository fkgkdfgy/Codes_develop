/*
 * @Author: Liu Weilong
 * @Date: 2020-09-23 16:08:13
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-02-25 09:33:43
 * @FilePath: /3rd-test-learning/10. utils/tictoc/tic_toc-test.cpp
 * @Description: tic_toc 计时器测试
 */

// chrono 来自C++11 cmake 需要手动进行设置

#include <chrono>
#include <iostream>
#include "tic_toc.hpp"
using namespace std;

int main()
{
    auto start = chrono::high_resolution_clock::now();
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> diff = end - start;
    int ms = std::chrono::duration_cast<chrono::milliseconds>(diff).count();
    {
        TicToc tic("sss");
        cout<<tic.checkprint_nano()<<endl;
        cout<<tic.checkprint_milli()<<endl;
        tic.checkprint_micro();
        tic.Toc();
    }
    cout<< ms<<endl;
    return 0;
}
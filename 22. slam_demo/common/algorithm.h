/*
 * @Author: Liu Weilong
 * @Date: 2021-02-19 11:59:32
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-02-19 12:10:54
 * @Description: 
 * project 的内容第一步，使用Algorithm 来进行算法计算用时的统计
 */
#include <iostream>
#include <vector>

using namespace std;

class Algorithm
{
       
    // 计时部分
    void tic();
    void toc();

    // static TicToc;
    vector<unsigned int> time_arrays_;
};
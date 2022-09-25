/*
 * @Author: Liu Weilong
 * @Date: 2020-09-09 18:47:30
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2020-11-09 19:37:18
 * @Description: 测试一下 Eigen 里Vector Array 相同读值的手法
 */

/**
 * @brief 测试结果就是array 和vector 都可以从()来进行读取
*/

#include <iostream>
#include "Eigen/Eigen"

using namespace std;

int main()
{
    Eigen::Vector2i tmp_1(1,1);
    Eigen::Array2i  tmp_2(1,1);

    cout<<"from vector  "<<tmp_1(0)<<"   "<<tmp_1(1)<<endl;
    cout<<"from array  "<<tmp_2(0)<<"   "<<tmp_2(1)<<endl;
}

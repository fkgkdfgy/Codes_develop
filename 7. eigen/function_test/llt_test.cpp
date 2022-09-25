/*
 * @Author: Liu Weilong
 * @Date: 2020-10-30 07:04:07
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2020-10-30 07:08:29
 * @Description:  用于测试LLt
 */

#include "eigen3/Eigen/Eigen"

#include <iostream>
using namespace std;

int main()
{
    Eigen::Matrix4d raw_matrix = Eigen::Matrix4d::Identity() * 9;
    
    Eigen::LLT<Eigen::Matrix4d> llt(raw_matrix);
    
    // 这里需要添加一个 toDenseMatrix() 不然形式不匹配
    cout<< llt.matrixL().toDenseMatrix()<<endl;
}
/*
 * @Author: Liu Weilong
 * @Date: 2020-11-12 07:44:41
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2020-11-12 07:48:41
 * @Description:  测试一下 矩阵模板 transpose 没有的情况
 */

#include "Eigen/Eigen"
#include <iostream>
using namespace std;

template <typename T, int N, int M>
void transposeTest(Eigen::Matrix<T,N,M> & m1)
{
    cout<<"the original matrix is "<<endl
        << m1<<endl;
    cout<<"the transpose matrix is "<<endl
        << m1.transpose()<<endl;
}

int main()
{
    Eigen::Matrix3f test;
    transposeTest(test);
    return 0;
}
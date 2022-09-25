/*
 * @Author: Liu Weilong
 * @Date: 2021-06-12 23:01:18
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-06-12 23:43:53
 * @Description: 
 * 
 * SIM BASALT margin 
 * 其实就是对阵矩阵转移
 * 基本测试完成
 */

#include <iostream>
#include "eigen3/Eigen/Eigen"

using namespace std;
using namespace Eigen;

static const int N = 10;
static const int n = 4;
int main()
{
    // Generate Sim Matrix
    Matrix<int,N,N> mat10;
    Matrix<int,n,n> mat44;
    mat10.setZero();
    mat44.setOnes();
    mat10.topLeftCorner(4,4) = mat44 * 4;
    mat10.bottomRightCorner(4,4) = mat44 * 10;

    Matrix<int,10,1> vec10;

    cout<<"矩阵转意之前"<<endl<<mat10<<endl;
    vec10<<4,5,6,7,0,1,2,3,8,9;

    const Eigen::PermutationWrapper<Eigen::Matrix<int, N, 1>> p(
    vec10);

    const Eigen::PermutationMatrix<N> pt = p.transpose();
    
    mat10.applyOnTheLeft(pt);
    mat10.applyOnTheRight(p);
    
    cout<<"矩阵转移之后"<<endl<<mat10<<endl;


}


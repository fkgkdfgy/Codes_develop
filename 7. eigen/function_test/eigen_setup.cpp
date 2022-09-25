/*
 * @Author: Liu Weilong
 * @Date: 2021-01-20 09:28:24
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-01-20 09:30:03
 * @FilePath: /3rd-test-learning/7. eigen/function_test/eigen_setup.cpp
 * @Description: 
 */
#include <iostream>
#include "eigen3/Eigen/Eigen"

using namespace std;

int main()
{
    Eigen::Vector3d one_row(1.0,2.0,3.0);
    Eigen::Matrix<double,1,9> temp;
    temp<<one_row.transpose(),one_row.transpose(),one_row.transpose();
    cout<<"the temp is "<<temp<<endl;
}
/***
 * 
 *  主要是为了测试Eigen的线性方程求解
 * 
 * 
 */

#include <iostream>
#include "Eigen/Eigen"

using namespace std;

int main()
{
    Eigen::Matrix4d tmp_H = Eigen::Matrix4d::Random();
    Eigen::Vector4d tmp_g = Eigen::Vector4d::Random();

    // LDLT
    auto tmp_result = tmp_H.ldlt().solve(tmp_g);

    // Cholesky
    
    tmp_result = tmp_H.llt


}
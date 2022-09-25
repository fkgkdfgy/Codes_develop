/*
 * @Author: Liu Weilong
 * @Date: 2021-01-07 20:12:27
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-01-07 20:16:44
 * @FilePath: /3rd-test-learning/7. eigen/matrixXd_map.cpp
 * @Description:  用于测试 map 和 Eigen::MatrixXd 的使用
 */
#include <map>
#include "Eigen/Eigen"

using namespace std;

int main()
{
    std::map<int,Eigen::MatrixXd> aaaa;
    Eigen::MatrixXd m(4,4);
    m.setIdentity();
    aaaa[1] = m;
    m = aaaa[1];
}
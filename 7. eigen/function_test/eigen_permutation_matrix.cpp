/*
 * @Author: Liu Weilong
 * @Date: 2021-06-12 19:04:40
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-06-12 22:56:52
 * @Description: 
 * 
 * 测试permutation matrix
 * 
 * 
 */
#include <iostream>
#include <eigen3/Eigen/Eigen>

using namespace std;
using namespace Eigen;

int main()
{
    Eigen::Matrix<double,10,1> vector10d;
    for(int i=0;i<10;i++)
    {
        vector10d(i)=9-i;
    }
    cout<<vector10d.transpose()<<endl;
    
    Eigen::PermutationWrapper<Eigen::Matrix<double,10,1>> p(vector10d);

    cout<<p.toDenseMatrix()<<endl;
}
/*
 * @Author: Liu Weilong
 * @Date: 2020-11-08 21:15:52
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2020-11-08 21:34:39
 * @Description: 测试 关于 Eigen::Map<Eigen::Matrxi<T,N,M>> 能否给一个 Matirx<T,M,> 进行传参， 
 *               结果就是不行
 * 
 * 
 * 
 *               并且查看是否有别的方法拯救一下
 */

#include "Eigen/Eigen"
#include <iostream>

using namespace std;

template<typename T,int N,int M>
bool print(Eigen::Matrix<T,N,M> & target_matrix)
{
    for(int row=0;row<N;row++)
    {
        for(int col = 0;col<M;col++)
        {
            cout<< target_matrix(row,col)<<" ";
        }
        cout<< endl;
    }
    cout<< endl;
    return true;
}

int main()
{
    Eigen::Matrix<double,6,7> matrix;
    double k[3]{1,1,1};
    double * k_ptr = k;
    Eigen::Map<Eigen::Matrix<double,3,1>> k_map(k);
    Eigen::Matrix<double,3,1> test_matrix = k_map;
    
    print(test_matrix);  // Eigen::Map 不能给Eigen::Matrix 传参
}
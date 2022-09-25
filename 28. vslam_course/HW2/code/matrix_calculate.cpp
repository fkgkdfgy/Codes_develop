/*
 * @Author: Liu Weilong
 * @Date: 2021-01-22 18:44:13
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-01-25 08:31:44
 * @Description: 
 */

#include <iostream>

#include "Eigen/Eigen"

using namespace std;

// LDU 
// template <typename T,int M,int N>
// bool GaussianElimination(const Eigen::Matrix<T,M,N> raw_matrix,
//                          Eigen::Matrix<T,M,M>& L,
//                          Eigen::Matrix<T,M,M>& D,
//                          Eigen::Matrix<T,M,N>& U)
// {

//     using Row = Eigen::Matrix<T,1,N>;
//     using Col = Eigen::Matrix<T,M,1>;

//     Eigen::Matrix<T,M,N> raw_copy = raw_matrix;
//     Eigen::Matrix<T,M,M> L_matrix = Eigen::Matrix<T,M,M>::Identity();
    
//     // 求 L
//     for(int i=0;i<N;i++)
//     {
//         // 如果 出现对角线 上元素为零的情况
//         if(raw_matrix(i,i) == 0)
//         {
//             for(int j=i;j<M;j++)
//             {
//                 if(raw_matrix(j,i)!=0)
//                 {
//                     Row row_old = raw_matrix.row(i);
//                     Row row_new = raw_matrix.row(j);
//                     raw_matrix.row(i) = row_new;
//                     raw_matrix.row(j) = row_old;
//                     Row L_row_old = L_matrix().row(i);
//                     Row L_row_new = L_matrix().row(j);
//                     L_matrix().row(i) = L_row_new;
//                     L_matrix().row(j) = L_row_old;
//                     break;
//                 }
//             }
//         }

//         if(raw_matrix(i,i)==0)
//         break;
        
//         for(int j=i+1;j<M;j++)
//         {
            
//             double scale = -1*raw_matrix(j,i)/raw_matrix(i,i);
//             Row row = raw_matrix.row(i);
//             raw_matrix.row(j) += scale* row;
//             Row L_row = L_matrix().row(i);
//             L_matrix.row(j) += scale * L_row;
//         }
//     }
// }




int main()
{
    Eigen::Matrix<double,3,3> temp = Eigen::Matrix<double,3,3>::Ones()*3;
    Eigen::MatrixXd A = Eigen::MatrixXd::Random(100,100);
    Eigen::MatrixXd b = Eigen::MatrixXd::Random(100,1);
    
    // Cholesky 分解
    Eigen::MatrixXd ATA = A.transpose()*A;
    while(ATA.determinant()<1e-9)
    {
        A = Eigen::MatrixXd::Random(100,100);
        ATA = A.transpose()*A;
    }
    Eigen::LLT<Eigen::MatrixXd> llt(A.transpose()*A);
    auto L = llt.matrixL();
    auto x1= llt.solve(b);
    cout<<"the part of x is "<<x1.matrix()<<endl;

    // QR 分解
    Eigen::HouseholderQR<Eigen::MatrixXd> qr(ATA);
    auto x3 = qr.solve(b);
    cout<<"the part of x is "<<x3.matrix()<<endl;
}




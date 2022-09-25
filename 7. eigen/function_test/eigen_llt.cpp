/*
 * @Author: Liu Weilong
 * @Date: 2020-10-17 22:20:19
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2020-10-17 22:22:13
 * @Description:  Eigen 的LLT 求解器
 *                2021/7/31 现在看来当时的使用是存在问题的
 */
#include <iostream>
#include "Eigen/Eigen"
using namespace Eigen;
using namespace std;
int main()
{

    MatrixXd A(3,3);
    A << 4,-1,2, -1,6,0, 2,0,5;
    cout << "The matrix A is" << endl << A << endl;

    LLT<MatrixXd> lltOfA(A); // compute the Cholesky decomposition of A
    MatrixXd L = lltOfA.matrixL(); // retrieve factor L  in the decomposition
    // The previous two lines can also be written as "L = A.llt().matrixL()"

    cout << "The Cholesky factor L is" << endl << L << endl;
    cout << "To check this, let us compute L * L.transpose()" << endl;
    cout << L * L.transpose() << endl;
    cout << "This should equal the matrix A" << endl;

}

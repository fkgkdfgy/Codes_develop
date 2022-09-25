/*
 * @Author: Liu Weilong
 * @Date: 2020-12-07 15:40:30
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2020-12-07 15:56:28
 * @FilePath: /3rd-test-learning/7. eigen/alignment_problem.cpp
 * @Description: 用于测试 Eigen的 Alignment Problem 字节对齐的问题
 */


#include "Eigen/Eigen"

class Foo
{
  //...
  public:
  Foo(const Eigen::Vector4d & a):
      v(a),v1(a),v2(a),v3(a),v4(a),v5(a),v6(a){}
  Eigen::Vector4d v;
  Eigen::Vector4d v1;
  Eigen::Vector4d v2;
  Eigen::Vector4d v3;
  Eigen::Vector4d v4;Eigen::Vector4d v5;
  Eigen::Vector4d v6;
  //...
};
//...
int main()
{
    Foo *foo = new Foo(Eigen::Vector4d::Zero());
}

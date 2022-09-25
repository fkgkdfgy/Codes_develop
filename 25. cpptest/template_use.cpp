/*
 * @Author: Liu Weilong
 * @Date: 2021-01-07 17:10:21
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-01-07 17:20:58
 * @FilePath: /3rd-test-learning/28. cpptest/template_use.cpp
 * @Description: 
 */
#include <iostream>
#include "Eigen/Eigen"
using namespace std;

template<typename A,int C>
class Base
{
    public:
    // 没有这一句的话是无法得到内部 A 的类型的
    using Type =A;
    static const int Dimension = C;
    Eigen::Matrix<double,Dimension,Dimension> content_;
    A aaa_;
};
template<typename A,int C>
class Derived:public Base<A,C>
{
    public:
    
};
using BaseA3 = Base<double,3>;
using DerivedA3 = Derived<double,3>;
int main()
{
    Derived<double,3> a;
    // DerivedA3::Dimension          这些值都是可以得到的
    // DerivedA3::Type               尽管没有在Derived 内部进行声明
    return 0;
}
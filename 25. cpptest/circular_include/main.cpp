/*
 * @Author: Liu Weilong
 * @Date: 2021-01-26 11:36:55
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-03-15 13:53:44
 * @FilePath: /3rd-test-learning/25. cpptest/circular_include/main.cpp
 * @Description: 
 * 
 * 主要目的：
 * １. 测试循环ｉｎｃｌｕｄｅ
 * ２. 测试循环include 的时候出现的field incomplete type 的问题：后来发现问题的原因是 前向声明的时候，需要用指针或者引用
 *     具体见A.h 的样例
 * 
 * 
 * 
 * 
 */
#include "B.h"

int main()
{
    B b1;
    b1.content = 100;
    B b2;
    b2.content = 0;
    A a;
    a.content = 9999;
    a.getB(b1);
    a.getB(b2);
    
}
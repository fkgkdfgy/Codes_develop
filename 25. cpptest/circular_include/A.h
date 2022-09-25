/*
 * @Author: Liu Weilong
 * @Date: 2021-01-24 17:27:28
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-03-15 13:54:53
 * @FilePath: /3rd-test-learning/25. cpptest/circular_include/A.h
 * @Description: 
 * 
 * 
 * 
 * 
 * 
 */

#ifndef _A_
#define _A_

#include <iostream>

using namespace std;

class B;

class A
{
    public:
    // B bp; 无法通过编译
    B* bp;//可以通过编译 
    void getB(B& b);
    int content;
};

#endif _A_
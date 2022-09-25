/*
 * @Author: Liu Weilong
 * @Date: 2021-01-24 17:27:33
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-03-05 10:15:03
 * @FilePath: /3rd-test-learning/25. cpptest/circular_include/B.h
 * @Description: 
 */
#include "A.h"

class B
{
    public:
    A* bp;
    
    ~B(){cout<<"B is been removed"<<endl;}

    int content;
};
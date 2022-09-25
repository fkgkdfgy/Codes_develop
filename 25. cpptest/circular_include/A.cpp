/*
 * @Author: Liu Weilong
 * @Date: 2021-03-15 13:50:35
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-03-15 13:54:16
 * @FilePath: /3rd-test-learning/25. cpptest/circular_include/A.cpp
 * @Description: 
 */

#include "A.h"
#include "B.h"

void A::getB(B& b)
{
    // cout<< b.content<<endl;
    if(bp!=nullptr)
    {
        // delete bp;
        // bp = new B(b);
        bp = &b;
        bp->content;
    }else{
        // bp = new B(b);
        bp = &b;
    }
}
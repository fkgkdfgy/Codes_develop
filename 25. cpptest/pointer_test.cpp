/*
 * @Author: Liu Weilong
 * @Date: 2021-02-23 10:17:39
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-02-23 10:20:46
 * @FilePath: /3rd-test-learning/25. cpptest/pointer_test.cpp
 * @Description: 
 */
#include <iostream>

using namespace std;
class Base
{
    int a,b,c;
};


int main()
{
    Base * a_p = new Base();
    Base * b_p = new Base();
    
    a_p == b_p;
    
    cout<<"a_p: "<<a_p<<endl;
    cout<<"b_p: "<<b_p<<endl;
    bool f = (a_p > b_p);
    cout<<f<<endl;
}
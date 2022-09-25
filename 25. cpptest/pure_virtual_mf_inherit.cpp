/*
 * @Author: Liu Weilong
 * @Date: 2021-01-07 16:14:36
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-01-07 16:23:23
 * @FilePath: /3rd-test-learning/28. cpptest/pure_virtual_mf_inherit.cpp
 * @Description: 这里主要是为了测试 基类指针能不能是一个有纯虚函数的类
 *               事实说明是可以的 哈哈哈哈哈哈哈哈 g2o 可以写的简单一些了
 */

#include <iostream>


using namespace std;
class Base
{   
    public:
    virtual void print() = 0;
};
class Derived :public Base
{
    public:
    virtual void print() override {cout<<"aaaa"<<endl;}
};

int main(int argc,char **argv)
{
    Base *aaa = new Derived();
    aaa->print();

    return 0;
}
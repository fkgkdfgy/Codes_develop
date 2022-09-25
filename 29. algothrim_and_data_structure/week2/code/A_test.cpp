/*
 * @Author: Liu Weilong
 * @Date: 2021-02-22 19:55:32
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-02-23 07:31:57
 * @Description: 
 * 
 * 文件的主要目的是:
 * 寻找sequence_list 编译不过的原因：
 * 最后发现是C++ 的模板继承，二阶段查找机制的问题
 * 
 * 
 * 
 */
#include <iostream>

using namespace std;

template<typename T>
class A
{
    public:
    A(int C_temp):C(C_temp){}
    int C;
    virtual void print(T c) =0;
};

template<typename T1>
class B:public A<T1>
{
    public:
    B(int C);
    virtual void print(T1 c);
};

template<typename T1>
B<T1>::B(int C):A<T1>(C){   // 这里构造函数  A<T1> 不能写成A
    T1 a;
    print(a);
}
template<typename T1>
void B<T1>::print(T1 c)
{
    cout<<"print c: "<<c<<endl;
    cout<<"print C: "<<this->C<<endl;// 加上 this-> 就可以找到C了
    // cout<<"print C: "<<C<<endl; 这么写会显示找不到C  
}

int main()
{
    B<double> b(123);
}


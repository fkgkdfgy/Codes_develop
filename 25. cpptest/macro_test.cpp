/*
 * @Author: Liu Weilong
 * @Date: 2020-12-04 09:42:23
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-01-24 16:59:17
 * @Description: 用于测试宏
 */
#define A(n) A##n
#define A2 vector
#include <iostream>
#include <string>
#include <vector>
using namespace std;
using A1 = string;

#define Binlier(n) Binlier##n

#define returnContent(n)\
Binlier##n * returnContent##n() {return &c##n;}

#define USE_THIS

class B
{
    public:
    using Binlier1 = int;
    using Binlier2 = float;
    using Binlier3 = string;
    void show()
    {
        C c;
        cout<<*c.returnContent1()<<endl;
        cout<<*c.returnContent2()<<endl;
        cout<<*c.returnContent3()<<endl;
    }
    class C
    {
        public:
        returnContent(1)
        returnContent(2)
        returnContent(3)
        Binlier1 c1 = 1;
        Binlier2 c2 = 10.0;
        USE_THIS Binlier3 c3 = "string";
    };
};


int main()
{
    A2<A(1)> vStr{"1","2","3"};
    for(auto Str:vStr)
    {
        cout<<Str<<endl;
    }
    B b;
    b.show();
    
}

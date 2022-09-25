/*
 * @Author: Liu Weilong
 * @Date: 2020-09-27 18:58:49
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2020-10-09 09:27:11
 * @FilePath: /3rd-test-learning/13. boost-serialization/derived-base-serialization.cpp
 * @Description: 现在的版本是一个derived-base的版本
 *               也就是如何序列化一个子类
 */

#include "boost/archive/text_iarchive.hpp"
#include "boost/archive/text_oarchive.hpp"

#include <iostream>
#include <fstream>

using namespace std;

class Base
{
    public:
    friend class boost::serialization::access;
    Base(){}
    Base(int tmp1,int tmp2):Content_1(tmp1),Content_2(tmp2){}
    // serial part
    template <typename T>
    void serialize( T & ar, unsigned int version)
    {
        ar & Content_1;
        ar & Content_2;
    }
    int get1(){return Content_1;}
    int get2(){return Content_2;}
    private:
    int Content_1;
    int Content_2;
};

class Derived :public Base
{
    public:
    friend class boost::serialization::access;
    

    Derived(){}

    Derived(int tmp12,int tmp3):Base(tmp12,tmp12),Content_3(tmp3){}

    // 对于derived 对象进行serialization 的方法
    template <typename T>
    void serialize(T & ar, unsigned int version)
    {
        ar& boost::serialization::base_object<Base>(*this);
        ar& Content_3;
    }
    void show()
    {
        cout<<Content_3<<endl;
        cout<<get1()<<endl;
        cout<<get2()<<endl;
    }
    private:
    int Content_3;

};


int main()
{
    std::string filename = "/home/lwl/workspace/3rd-test-learning/11. boost-serialization/build/file";

    std::ofstream ofs(filename.c_str());
    const Derived d(35,59);

    {
        boost::archive::text_oarchive oa(ofs);
        oa<<d;
    }

    // 第三步 进行提取
    Derived new_d;
    // 没有进行初始化的打印
    new_d.show();
    {
        std::ifstream ifs(filename.c_str());
        boost::archive::text_iarchive ia(ifs);
        ia>>new_d;
    }
    // 进行过反序列化的打印
    new_d.show();
    return 0;
}
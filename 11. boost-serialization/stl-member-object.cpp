/*
 * @Author: Liu Weilong
 * @Date: 2020-09-27 19:29:05
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-01-24 09:49:33
 * @FilePath: /3rd-test-learning/11. boost-serialization/stl-member-object.cpp
 * @Description: 
 * 
 *               重要内容提示：
 *               1. 如果要想进行 STL 容器的序列化
 *                  需要 include boost/serialization/${stl_container}.cpp
 *                  i.e. boost/serialization/vector.hpp
 *                 
 * 
 *               用于测试序列化 C++ STL 容器 vector 为例其他内容相同
 *               1. 序列化vector<内置类型> 
 *               2. 序列化vector<自定义class 类型>
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

// boost/serializatoin/含有各种 stl 的容器序列化适配 
#include "boost/serialization/vector.hpp"


#include "boost/archive/text_iarchive.hpp"
#include "boost/archive/text_oarchive.hpp"


using namespace std;

struct VectorSerializationTest{
    
    friend boost::serialization::access;

    VectorSerializationTest()
    {
        for(int i =0;i<10;i++)
        {
            int_ar .push_back(i);
            name_ = "sssssss";
        }
    }

    template<typename T>
    void serialize(T & ar,unsigned int version)
    {
        ar & int_ar;
    }
    
    string name_;
    vector<int> int_ar;
};

struct VectorSerializationTestClassVersion
{
    VectorSerializationTestClassVersion()
    {
        vv_ar.clear();
        vv_ar.push_back(VectorSerializationTest());
        vv_ar.push_back(VectorSerializationTest());
    }

    friend boost::serialization::access;
    template<typename T>
    void serialize(T & ar,unsigned int version)
    {
        ar & vv_ar;
    }

    vector<VectorSerializationTest> vv_ar;
};
int main()
{
    std::string filename = "/home/lwl/workspace/3rd-test-learning/11. boost-serialization/build/file";

    std::ofstream ofs(filename.c_str());
    const VectorSerializationTestClassVersion t;

    {
        boost::archive::text_oarchive oa(ofs);
        oa<<t;
    }

    // 第三步 进行提取
    VectorSerializationTestClassVersion new_g;
    {
        std::ifstream ifs(filename.c_str());
        boost::archive::text_iarchive ia(ifs);
        ia>>new_g;
    }

    cout << new_g.vv_ar.front().name_<<endl;
    for(auto i :new_g.vv_ar.front().int_ar)
    {
        cout<<i<<endl;
    }
    cout << new_g.vv_ar.back().name_<<endl;
    for(auto i :new_g.vv_ar.back().int_ar)
    {
        cout<<i<<endl;
    }
    return 0;
}



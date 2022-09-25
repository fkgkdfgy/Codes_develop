/*
 * @Author: Liu Weilong
 * @Date: 2020-09-27 17:58:45
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2020-09-27 18:45:48
 * @FilePath: /3rd-test-learning/13. boost-serialization/boost-serializatoin-simple-case-test.cpp
 * @Description: 用于练习boost serialization 这里的例子是一个入侵式的例子
 */
#include <iostream>
#include <fstream>
#include <string>
// boost 序列化库
#include "boost/archive/text_oarchive.hpp"
#include "boost/archive/text_iarchive.hpp"

using namespace std;

class Ah{};
class gps_position
{
    public:
    
    // 第一步 定义序列化内容
    // 包括序列化子类声明  和 序列化方式
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive & ar,const unsigned int version)
    {
        ar & degrees;
        ar & minutes;
        ar & seconds;
    }


    int degrees;
    int minutes;
    float seconds;

    gps_position(){}
    gps_position(int d,int m ,float s):degrees(d),minutes(s),seconds(s){}
    

};

int main()
{
    // 第二步 进行存储
    // 这里也是在进行测试如何 使用相对路径
    std::string filename = "/home/lwl/workspace/3rd-test-learning/11. boost-serialization/build/file";

    std::ofstream ofs(filename.c_str());
    const gps_position g(35,59,24.567f);

    {
        boost::archive::text_oarchive oa(ofs);
        oa<<g;
    }

    // 第三步 进行提取
    gps_position new_g;
    {
        std::ifstream ifs(filename.c_str());
        boost::archive::text_iarchive ia(ifs);
        ia>>new_g;
    }

    cout << new_g.degrees<<endl;
    cout << new_g.minutes<<endl;
    cout << new_g.seconds<<endl;
    return 0;

}
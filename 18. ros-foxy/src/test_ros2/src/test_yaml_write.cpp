#include <iostream>
#include <fstream>
#include <string>

#include "yaml-cpp/yaml.h"

using std::string;
using std::cout;
int main()
{
    // 读测试
    string filename_ = "/home/sunny/Desktop/test_ROS2/src/test_ros2/param/test_yaml_write.yaml";
    auto config_ = YAML::LoadFile(filename_);
    cout<<"the name is "<< config_["Person1"]["name"].as<string>()<<std::endl;
    cout<<"the height is "<<config_["Person1"]["height"].as<float>()<<std::endl;
    cout<<"the age is "<<config_["Person1"]["age"].as<int>()<<std::endl;

    // 类型测试

    if(config_["Person1"].IsMap())
    {
        std::cout<<"Person1 is a Map"<<std::endl;
    }
    if(config_["Sequence_test"].IsSequence())
    {
        std::cout<<"Sequence_test is a Sequence"<<std::endl;
    }
    if(config_["Person1"]["name"].IsDefined())
    {
        std::cout<<"Person1 name is defined"<<std::endl;
    }


    // 写测试
    // 单独添加    
    std::string filename_save_ = "/home/sunny/Desktop/test_ROS2/src/test_ros2/param/test_yaml_write_save.yaml";
    std::ofstream fstream_(filename_save_,std::fstream::out | std::ios_base::trunc);
    config_["lastRead"] = "LWL";
    fstream_<<config_<<std::endl;

    fstream_<<"---------------------------------------"<<std::endl;
    // 添加类结构
    // 从两次<< 的结果来看config 内部存的是所有的值
    auto config_temp_ = config_["Person2"];
    config_temp_["name"] = "TNW";
    config_temp_["age"] = 24;
    fstream_<<config_<<std::endl;
    fstream_<<"---------------------------------------"<<std::endl;  
    // 修改测试
    // 失败
    config_["lastRead"] = "TNW";
    fstream_<<config_["lastRead"]<<std::endl;
    fstream_<<"---------------------------------------"<<std::endl; 

    // 内容修改二次尝试
    // Map 修改成功
    config_["Person1"]["name"] = "TNW";
    fstream_<<config_<<std::endl;
    fstream_<<"---------------------------------------"<<std::endl; 
    // Sequence 修改
    config_["Sequence_test"][1] = "ggg";
    fstream_<<config_<<std::endl;
    fstream_<<"---------------------------------------"<<std::endl; 

    // 打印测试
    // 之前都是把config 整个进行打印这里只打印一部分
    // 成功但是需要舍弃当上一层的Person1 包装
    YAML::Node config_tmp_ = config_["Person1"];
    fstream_<<config_tmp_<<std::endl;
    fstream_<<"---------------------------------------"<<std::endl; 

    // 如果需要部分进行打印外层加包装
    if (config_["Test_makeup"].IsMap())
    {
        std::cout<<"Test_makeup is a Map"<<std::endl;
    }
    else if(config_["Test_makeup"].IsSequence())
    {
        std::cout<<"Test_makeup is a Map"<<std::endl;
    }
    
    config_tmp_.reset();
    config_tmp_.push_back(config_["Test_makeup"]);
    fstream_<<config_tmp_<<std::endl;
    fstream_<<"---------------------------------------"<<std::endl; 


    return 0;
}
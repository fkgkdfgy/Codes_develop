/*
 * @Author: Liu Weilong
 * @Date: 2021-01-04 10:38:19
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-01-04 15:09:51
 * @FilePath: /3rd-test-learning/11. utils/data_collector/data_collect_test.cpp
 * @Description: 
 */
#include <memory>

#include "data_collector_T.h"

int main()
{
    const string store_path= "/home/sunny/workspace_private/3rd-test-learning/11. utils/data_collector/T_collection.txt";
    const string info_name = "Transform";

    // 偶然发现的一个问题  就是如果 是fstream 而不是 ofstream 的话
    // std::ios_base::trunc 会报错 无法打开文件
    // ofstream fout_;

    // fout_.open(store_path.c_str(),std::ios_base::trunc);
    // if(!fout_.is_open())
    // {
    //     std::cerr<<"the file openning meets some problems11111111111!!"<<endl;
    //     abort();
    // }

    std::shared_ptr<data_collector::DataCollectorT> dc_ptr;
    dc_ptr = make_shared<data_collector::DataCollectorT>(store_path,info_name);
    
    for(int i=0;i<100;i++)
    {
        Eigen::AngleAxisd aa(i* 0.01,Eigen::Vector3d(0,0,1));
        Eigen::Vector3d transVec(i*0.1,i*0.2,i*0.3);
        Eigen::Matrix4d t = Eigen::Matrix4d::Zero();      
        t.block<3,3>(0,0) = aa.matrix();
        t.block<3,1>(0,3) = transVec;
        (*dc_ptr)<<pair<int,Eigen::Matrix4d>(i,t);
    }
    cout<<dc_ptr->ShowSize()<<endl;
}
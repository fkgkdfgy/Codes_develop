/*
 * @Author: Liu Weilong
 * @Date: 2020-12-07 14:39:06
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2020-12-14 14:33:11
 * @FilePath: /3rd-test-learning/11. utils/dataset_fitting/save_result.h
 * @Description:  转换到标准的真值格式
 */
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <iomanip>
#include <algorithm>
#include "Eigen/Eigen"

using namespace std;

// TUM 保存格式
template<typename T>
void savePath(fstream & fout, const Eigen::Matrix4d & Tfw, T timestamp)
{
    if(!fout.is_open())
    {
        std::cerr<<"the file is not opened !"<<endl;
        std::abort();
    }
    Eigen::Vector3d P = Tfw.block<3,1>(0,3);
    Eigen::AngleAxisd aa(Tfw.block<3,3>(0,0));
    Eigen::Quaterniond Q(aa);
    fout <<timestamp<< " ";
    fout.precision(5);
    fout    << P.x() << " "
            << P.y() << " "
            << P.z() << " "
            << Q.w() << " "
            << Q.x() << " "
            << Q.y() << " "
            << Q.z() << " "
            << endl;
}

void EuRocTimeStamp(string & data_line)
{
    auto n = data_line.find(',');
    auto timestamp = data_line.substr(0,n).substr(5);
    auto timestamp_d = double(stoull(timestamp))*1e-9;
    std::stringstream ss;
    ss << std::setprecision(15) << timestamp_d;
    auto str = ss.str(); 
    string new_str = str+data_line.substr(n);
    cout<<new_str<<endl;
    data_line = new_str;
}

void EuRocTimeStampORB(string & data_line)
{
    auto n = data_line.find(',');
    auto timestamp = data_line.substr(0,n);
    timestamp=timestamp.substr(0,timestamp.length()-3);
    auto timestamp_d = double(stoull(timestamp))*1e-6;
    std::stringstream ss;
    ss << std::setprecision(16) << timestamp_d;
    auto str = ss.str(); 
    string new_str = str+data_line.substr(n);
    cout<<new_str<<endl;
    data_line = new_str;
}

bool EuRocToTUMInterf(const string & csv_path_in, const string& csv_path_out)
{
    ifstream fin(csv_path_in.c_str());                        // 打开文件流操作
    ofstream fout(csv_path_out.c_str());
    string data_line;
    if(!fin.is_open())
    {
        std::cout<<"[FATAL]: the csv_file_path is invalid. Please check the path "<<":"<<endl
                 << csv_path_in.c_str()<<endl;
        return false;
    }
    if(!fout.is_open())
    {
        std::cout<<"[FATAL]: the csv_file_path is invalid. Please check the path "<<":"<<endl
                 << csv_path_out.c_str()<<endl;
        return false;
    }


    while(getline(fin,data_line))
    {
        EuRocTimeStamp(data_line); 
        fout<<data_line<<endl;  
    }

    fin.close();
    fout.close();
}
bool EuRocToTUMORBInterf(const string & csv_path_in, const string& csv_path_out)
{
    ifstream fin(csv_path_in.c_str());                        // 打开文件流操作
    ofstream fout(csv_path_out.c_str());
    string data_line;
    if(!fin.is_open())
    {
        std::cout<<"[FATAL]: the csv_file_path is invalid. Please check the path "<<":"<<endl
                 << csv_path_in.c_str()<<endl;
        return false;
    }
    if(!fout.is_open())
    {
        std::cout<<"[FATAL]: the csv_file_path is invalid. Please check the path "<<":"<<endl
                 << csv_path_out.c_str()<<endl;
        return false;
    }
    while(getline(fin,data_line))
    {
        EuRocTimeStampORB(data_line);   
        fout<<data_line<<endl;  
    }
    fin.close();
    fout.close();
}

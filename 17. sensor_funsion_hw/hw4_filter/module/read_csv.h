/*
 * @Author: Liu Weilong
 * @Date: 2020-11-07 16:12:25
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2020-12-06 02:30:30
 * @Description:  
 *               为了给imu 进行性能分析 所以需要把csv 转换成 rosbag 的形式
 *               这里主要是使用 C++ 原声的io  对 csv 文件进行读取
 * 
 */
#pragma once
#include <iostream>
#include <fstream>
#include <limits>
#include <sstream>
#include <vector>
#include <string>
#include "Eigen/Eigen"
 
using namespace std;

bool readCSV(vector<std::string> & header, 
             vector<vector<double>> & data,
             std::string csv_path,int row = -1,bool have_header = true,char interval_char = ',')
{
    ifstream fin(csv_path.c_str());                        // 打开文件流操作
    stringstream ss;
    if(!fin.is_open())
    {
        std::cout<<"[FATAL]: the csv_file_path is invalid. Please check the path "<<":"<<endl
                 << csv_path.c_str()<<endl;
        return false;
    }

    header.clear();
    data.clear();
    if(have_header)
    {
        string line;
        string line_small;
        
        getline(fin,line);
        ss.str(line);   
        while(getline(ss,line_small,interval_char))
        {
            header.push_back(line_small);
        } 
        ss.clear();
    }

    data.resize(header.size());

    string data_line;
    int count=0;
    int count_thres = row;
    if(row<=0)
    {
        cout<<"[WARNING]: we will read whole csv file"<<endl;
        count_thres = numeric_limits<int>().max();
    }

    while(getline(fin,data_line))
    {
        string info;
        ss.str(data_line);
        for(int i =0;i<header.size();i++)
        {
            if(!getline(ss,info,interval_char))                            // 使用 ‘，’对一行数据进行分割
            {
                std::cout<<"[ERROR]: Maybe A broken file !"<<std::endl;
                return false;
            }
            data.at(i).emplace_back(atof(info.c_str()));
        }
        count++;
        if(count == count_thres)
        {
            break;
        }
        ss.clear();
    }

    cout<<"Complete Read CSV file"<<endl;
    return true;
    
}

template<typename T,int rows>
bool saveCSV(string path, const vector<Eigen::Matrix<T,rows,1>> & data,char interval_char = ' ')
{
    ofstream fs(path.c_str());
    if(!fs.is_open())
    {
        std::cerr<<"[ERROR]:there might be something wrong with the csv path!"<<endl;
        std::abort();
    }
}
 
// int main(int argc, char** argv)
// {
//     std::vector<string> headers;
//     std::vector<vector<double>> data;
//     string csv_path = "/home/lwl/workspace/HW/gnss-ins-sim/demo_motion_def_files/imu_simulation_hw/accel-0.csv";
//     readCSV(headers,data,csv_path,3);

//     for(auto& header:headers)
//     {
//         cout<<header<<endl;
//     }

//     for(auto & infos:data)
//     {
//         for(auto info:infos)
//         {
//             cout<<info<<" ";
//         }
//         cout<<endl;
//     }
// }
/*
 * @Author: Liu Weilong
 * @Date: 2021-05-21 13:48:31
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-05-23 22:56:35
 * @Description: 
 * 
 * 
 * data_loader 进行 imu 信息加载
 * 
 * 注意 这里的格式处理只是应对 imu-gnss-sim 这个项目
 * 
 * */

#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

// #include 


#include "Eigen/Eigen"
#include "opencv2/opencv.hpp"

using namespace std;


class DataLoader
{
    public:


    template<int N>
    class Info
    {
        public:

        static const int Dimension = N;

        vector<vector<double>> common_info;
        vector<Eigen::Matrix<double,N,1>> eigen_info;
        string info_name;
        bool TransformIntoEigenForm()
        {
            if(common_info.size()== 0)
                cout<<"[WARN] Info: the common_info is empty"<<endl;
            eigen_info.resize(common_info.size(),Eigen::Matrix<double,Dimension,1>());
            for(int i=0;i<common_info.size();i++)
            memcpy(eigen_info[i].data(),common_info[i].data(),sizeof(double)*Dimension); 
        }
    };

    class ElementInfo
    {
        public:
        Eigen::Vector3d ref_vel;
        Eigen::Vector3d ref_acc;
        Eigen::Vector3d ref_gyro;
        Eigen::Vector3d meas_acc;
        Eigen::Vector3d meas_gyro;
        Eigen::Vector3d posi;
        Eigen::Vector4d atti;
        double timestamp;
    };

    DataLoader(const string & paths_config):paths_config_(paths_config)
    {
        cv::FileStorage paths_config_node(paths_config.c_str(),cv::FileStorage::READ);

        cout<<"[LOG]Dataloader: start loading paths"<<endl;
        
        paths_config_node["WholePath"]>>whole_path_;

        paths_config_node["RefVelPath"]>>ref_vel_path_;
        paths_config_node["RefAccPath"]>>ref_acc_path_;
        paths_config_node["RefGyroPath"]>>ref_gyro_path_;

        paths_config_node["MeasAccPath"]>>meas_acc_path_;
        paths_config_node["MeasGyroPath"]>>meas_gyro_path_;

        paths_config_node["PosiPath"]>>posi_path_;
        paths_config_node["AttiPath"]>>atti_path_;

        paths_config_node["TimestampPath"]>>timestamp_path_;

        input_num_ = (int)paths_config_node["InputNum"];

        // 显示各个路径的内容
        cout<<"[LOG]Dataloader:WholePath: "<<whole_path_<<endl;
        cout<<"[LOG]Dataloader:RefVelPath: " << ref_vel_path_<<endl;
        cout<<"[LOG]Dataloader:RefAccPath: " << ref_acc_path_<<endl;
        cout<<"[LOG]Dataloader:RefGyroPath: "<<ref_gyro_path_<<endl;
        cout<<"[LOG]Dataloader:MeasAccPath: "<<meas_acc_path_<<endl;
        cout<<"[LOG]Dataloader:MeasGyroPath: "<< meas_gyro_path_<<endl;
        cout<<"[LOG]Dataloader:PosiPath: "<< posi_path_<<endl;
        cout<<"[LOG]Dataloader:AttiPath: "<<atti_path_<<endl;
        cout<<"[LOG]Dataloader:TimestampPath"<< timestamp_path_<<endl;
        output_count_ = 0;
        LoadInfo();
    }

    bool GetInfo(ElementInfo & output)const
    {
        if(output_count_>= ref_acc_info_.common_info.size())
        return false;

        output.ref_vel = ref_vel_info_.eigen_info[output_count_];
        output.ref_acc = ref_acc_info_.eigen_info[output_count_];
        output.ref_gyro = ref_gyro_info_.eigen_info[output_count_]/180.0*M_PI;
        output.meas_acc = meas_acc_info_.eigen_info[output_count_];
        output.meas_gyro = meas_gyro_info_.eigen_info[output_count_]/180.0*M_PI;
        output.posi = posi_info_.eigen_info[output_count_];
        output.atti = atti_info_.eigen_info[output_count_];
        
        output.timestamp = timestamp_info_.common_info[output_count_].front();
        output_count_++;
        return true;
    }

    void Reset()const {output_count_ = 0;};

    private:

    bool LoadInfo()
    {
        string syn_path;
        // load ref_acc
        syn_path = whole_path_ + ref_acc_path_;
        LoadCSV(syn_path,ref_acc_info_);
        ref_acc_info_.TransformIntoEigenForm();

        // load ref_vel
        syn_path = whole_path_ + ref_vel_path_;
        LoadCSV(syn_path,ref_vel_info_);
        ref_vel_info_.TransformIntoEigenForm();

        // load ref_gyro
        syn_path = whole_path_ + ref_gyro_path_;
        LoadCSV(syn_path,ref_gyro_info_);
        ref_gyro_info_.TransformIntoEigenForm();

        // load meas_acc
        syn_path = whole_path_ + meas_acc_path_;
        LoadCSV(syn_path,meas_acc_info_);
        meas_acc_info_.TransformIntoEigenForm();
        
        // load meas_gyro
        syn_path = whole_path_ + meas_gyro_path_;
        LoadCSV(syn_path,meas_gyro_info_);
        meas_gyro_info_.TransformIntoEigenForm();

        // load posi
        syn_path = whole_path_ + posi_path_;
        LoadCSV(syn_path,posi_info_);
        posi_info_.TransformIntoEigenForm();

        // load atti
        syn_path = whole_path_ + atti_path_;
        LoadCSV(syn_path,atti_info_);
        atti_info_.TransformIntoEigenForm();

        // load timestamp
        syn_path = whole_path_ + timestamp_path_;
        LoadCSV(syn_path,timestamp_info_);

        if(ref_acc_info_.common_info.size() == ref_gyro_info_.common_info.size() ==
           meas_acc_info_.common_info.size() == meas_gyro_info_.common_info.size() ==
           posi_info_.common_info.size() == atti_info_.common_info.size() ==
           timestamp_info_.common_info.size())
        return true;
    }
    
    template<int N>
    bool LoadCSV(const string & path,
                 Info<N> & output)
    {
        // 检测path 的合理性
        ifstream fin(path);
        if(!fin.is_open())
        {
            cerr<<"[FATAL]Dataloader:LoadCSV with invalid file path"<<endl;
            return false;
        }

        stringstream ss;

        string data_line;

        if(input_num_ <0)
        cout<<"[WARN]Dataloader: load all info from files"<<endl;


        // 用于去掉第一行的header
        
        getline(fin,data_line);

        int count = 0;

        std::vector<double> tmp_info;
        tmp_info.reserve(15);        
        while(getline(fin,data_line))
        {
            string info;
            ss.str(data_line);
 
            while(getline(ss,info,','))                            // 使用 ‘，’对一行数据进行分割
            {
                tmp_info.push_back(atof(info.c_str()));
            }
            output.common_info.push_back(tmp_info);
            count++;
            if(count == input_num_)
            {
                break;
            }
            ss.clear();
            tmp_info.clear();
        }

        cout<<"Complete Read CSV file"<<endl;
        return true;
    }

    
    private:


    string paths_config_;
    
    string whole_path_;

    string ref_vel_path_;
    string ref_acc_path_;
    string ref_gyro_path_;
    
    string meas_acc_path_;
    string meas_gyro_path_;
    
    string posi_path_;
    string atti_path_;

    string timestamp_path_;

    Info<3> ref_vel_info_;
    Info<3> ref_acc_info_;
    Info<3> ref_gyro_info_;
    
    Info<3> meas_acc_info_;
    Info<3> meas_gyro_info_;

    Info<3> posi_info_;
    Info<4> atti_info_;

    Info<1> timestamp_info_;

    int input_num_;
    mutable int output_count_;
};


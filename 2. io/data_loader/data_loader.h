/*
 * @Author: Liu Weilong
 * @Date: 2021-01-20 13:52:24
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-01-20 15:44:44
 * @FilePath: /3rd-test-learning/2. io/data_loader/data_loader.h
 * @Description: 
 *               一定配合 rosbag_convert 里面的LoadImgAndSaveInFile 一起使用
 * 
 *           
 */

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "opencv2/highgui/highgui.hpp"

using namespace std;


class DataLoader
{
    public:
    DataLoader(const string & all_path);
    
    bool LoadIndex(const string & index_left , const string & index_right);
    bool LoadImg(cv::Mat & img_l , cv::Mat & img_r);
    bool reset(){cur_index_=0;}
    private:

    bool TimeCheck(const vector<double> & idx1,const vector<double> & idx2);

    
    const string all_path_;
    
    string image_path_left_;
    string image_path_right_;
    string image_idx_left_;
    string image_idx_right_;
    
    vector<string> image_path_left_array_;
    vector<string> image_path_right_array_;
    vector<double> image_time_array_;
    unsigned int cur_index_ = 0;
};

DataLoader::DataLoader(const string & all_path):all_path_(all_path)
{
    cv::FileStorage config_fin;
    config_fin.open(all_path_.c_str(),CV_STORAGE_READ);
    if(!config_fin.isOpened())
    {
        std::cerr<<"[ERROR]: DataLoader cannot open the config file!"<<std::endl;
        abort();
    }

    config_fin["DataLoader.ImagePath.Left"]>>image_path_left_;
    config_fin["DataLoader.ImagePath.Right"]>>image_path_right_;
    config_fin["DataLoader.ImagePathIdx.Left"]>>image_idx_left_;
    config_fin["DataLoader.ImagePathIdx.Right"]>>image_idx_right_;


    LoadIndex(image_idx_left_ ,  image_idx_right_);
    
    cout<<"DataLoader.ImagePath.Left : "<<image_path_left_.c_str()<<endl;
    cout<<"DataLoader.ImagePath.Right : "<<image_path_right_.c_str()<<endl;
    cout<<"DataLoader.ImagePathIdx.Left : "<<image_idx_left_.c_str()<<endl;
    cout<<"DataLoader.ImagePathIdx.Right : "<<image_idx_right_.c_str()<<endl;

}

bool DataLoader::LoadIndex(const string & index_left , const string & index_right)
{
    ifstream fin;
    fin.open(index_left.c_str());
    if(!fin.is_open())
    {
        std::cerr<<"[ERROR]: DataLoader cannot load the index!"<<std::endl;
        abort();
        return false;
    }
    image_time_array_.reserve(5000);
    image_path_left_array_.reserve(5000);
    image_path_right_array_.reserve(5000);
    std::vector<double> temp_t;
    temp_t.reserve(5000);
    while(!fin.eof())
    {
        string s;
        getline(fin,s);
        if(!s.empty())
        {
            image_path_left_array_.push_back(image_path_left_+"/"+s);
            double t = std::atof(s.substr(0,s.length()-4).c_str());
            image_time_array_.push_back(t/1e9);
        }
    }

    fin.close();

    fin.open(index_right.c_str());
    if(!fin.is_open())
    {
        std::cerr<<"[ERROR]: DataLoader cannot load the index!"<<std::endl;
        abort();
        return false;
    }
    while(!fin.eof())
    {
        string s;
        getline(fin,s);
        if(!s.empty())
        {
            image_path_right_array_.push_back(image_path_right_+"/"+s);
            double t = std::atof(s.substr(0,s.length()-4).c_str());
            temp_t.push_back(t/1e9);
        }
    }
    fin.close();
    if(TimeCheck(temp_t,image_time_array_))
    return true;
    else
    {
        cerr<<"[FATAL]:Time idx is wrong"<<endl;
        abort();
        return false;
    }
}

bool DataLoader::LoadImg(cv::Mat & img_l , cv::Mat & img_r)
{
    if(cur_index_>=image_path_left_array_.size())
    return false;
    img_l = cv::imread(image_path_left_array_[cur_index_],CV_LOAD_IMAGE_UNCHANGED);
    img_r = cv::imread(image_path_right_array_[cur_index_],CV_LOAD_IMAGE_UNCHANGED);
    cur_index_ ++;
    return true;
}

bool DataLoader::TimeCheck(const vector<double> & idx1,const vector<double> & idx2)
{
    if(idx1.size()!=idx2.size())
    return false;

    for(int i=0;i<idx1.size();i++)
    {
        if(std::abs(idx1[i]-idx2[i])>3e-3)
        {
            cout<<"the error idx1 is "<< idx1[i]<<"  . the error idx2 is "<<idx2[i]<<endl;
            cout<<"the corresponding image is IMGL: "<<image_path_left_array_[i]<<endl;
            cout<<"the corresponding image is IMGR: "<<image_path_right_array_[i]<<endl;
            return false;
        }
    }

    return true;
}

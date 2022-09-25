/*
 * @Author: Liu Weilong
 * @Date: 2021-02-27 22:49:02
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-03-06 20:45:29
 * @Description: 
 */

#pragma once 
#include <iostream>
#include <vector>
#include <string>
#include <chrono>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "eigen3/Eigen/Eigen"
#include "sophus/se3.hpp"


using Corners = std::vector<Eigen::Vector2f,Eigen::aligned_allocator<Eigen::Vector2f>>;
using namespace std;

class TrackerBaseOptions
{
    public:
    TrackerBaseOptions(const std::string & config);
    int level_;
    int template_;
    float scale_;
    int max_iterations_;
};

TrackerBaseOptions::TrackerBaseOptions(const std::string &config)
{
    cv::FileStorage fin(config.c_str(),CV_STORAGE_READ);
    if(!fin.isOpened()) 
    {
        cerr<<"the config path is wrong!"<<endl;
        abort();
    }
    level_ = (int)fin["TrackerBaseOptions.Level"];
    template_ = (int)fin["TrackerBaseOptions.TemplateSize"];
    scale_ = (double)fin["TrackerBaseOptions.Scale"];
    max_iterations_ = (int)fin["TrackerBaseOptions.MaxIterations"];
}

class TrackerBase
{   
    public:
    
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    TrackerBase(const std::string & config);

    void SetInput(cv::Mat pre_img, const Corners& pre_corners);
    void SetTarget(cv::Mat cur_img);
    const Corners & GetCorners() const;
    bool CreatePyramid();
    virtual void Run();
    virtual void Impl()=0;
    
    Corners pre_corners_,cur_corners_;
    cv::Mat pre_img_,cur_img_;
    vector<cv::Mat> pyr_pre_img_,pyr_cur_img_;
    
    TrackerBaseOptions * options_ptr_;
};

TrackerBase::TrackerBase(const std::string & config)
{
    options_ptr_ = new TrackerBaseOptions(config);
}

void TrackerBase::SetInput(cv::Mat pre_img,const Corners & pre_corners)
{
    pre_img_ = pre_img.clone();
    pre_corners_ = pre_corners;
}

void TrackerBase::SetTarget(cv::Mat cur_img)
{
    cur_img_ = cur_img.clone();
}

const Corners & TrackerBase::GetCorners()const
{
    return cur_corners_;
}

bool TrackerBase::CreatePyramid()
{
    auto start = chrono::high_resolution_clock::now();
    if(pre_img_.empty()||cur_img_.empty())
    {
        cerr<<"the pre_img or cur_img is empty!"<<endl;
        return false;
    }
    pyr_pre_img_.resize(options_ptr_->level_);
    pyr_cur_img_.resize(options_ptr_->level_);
    pyr_pre_img_[0] = pre_img_;
    pyr_cur_img_[0] = cur_img_;
    for(int i=1;i<options_ptr_->level_;i++)
    {
        cv::resize(pyr_pre_img_[i-1],pyr_pre_img_[i],
                   cv::Size(pyr_pre_img_[i-1].cols*options_ptr_->scale_,pyr_pre_img_[i-1].rows*options_ptr_->scale_));
        cv::resize(pyr_cur_img_[i-1],pyr_cur_img_[i],
                   cv::Size(pyr_cur_img_[i-1].cols*options_ptr_->scale_,pyr_cur_img_[i-1].rows*options_ptr_->scale_));           
    }

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> diff = end - start;
    cout<<"Create Pyr : "<< diff.count()<<" s"<<endl;
    return true;
}

void TrackerBase::Run()
{
    auto start = chrono::high_resolution_clock::now();
    CreatePyramid();
    Impl();
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> diff = end - start;
    cout<<"Total: "<< diff.count()<<" s"<<endl;
}

/*
 * @Author: Liu Weilong
 * @Date: 2021-01-13 22:41:49
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-01-18 06:37:55
 * @Description: 
 */

#include "environment_builder.h"

_S_SLAM_DEMO_

EnvironmentBuilderOptions::EnvironmentBuilderOptions(const string & config_path)
{
    cv::FileStorage fin(config_path,CV_STORAGE_READ);
    if(!fin.isOpened())
    {
        cerr<<"[ERROR]: there is something wrong with the path"<<endl;
        abort();
    }
    fin["Environment.Xmin"]>>x_min_;
    fin["Environment.Ymin"]>>y_min_;
    fin["Environment.Zmin"]>>z_min_;
    fin["Environment.Xscale"]>>x_scale_;
    fin["Environment.Yscale"]>>y_scale_;
    fin["Environment.Zscale"]>>z_scale_;
    fin["Environment.Xinterval"]>>x_interval_;
    fin["Environment.Yinterval"]>>y_interval_;
    fin["Environment.Zinterval"]>>z_interval_;

    cout<<"Environment.Xmin : "<< x_min_<<endl;
    cout<<"Environment.Ymin : "<< y_min_<<endl;
    cout<<"Environment.Zmin : "<< z_min_<<endl;
    cout<<"Environment.Xscale : "<<x_scale_<<endl;
    cout<<"Environment.Yscale : "<<y_scale_<<endl;
    cout<<"Environment.Zscale : "<<z_scale_<<endl;
    cout<<"Environment.Xinterval : "<<x_interval_<<endl;
    cout<<"Environment.Yinterval : "<<y_interval_<<endl;
    cout<<"Environment.Zinterval : "<<z_interval_<<endl;
}


EnvironmentBuilder::EnvironmentBuilder(const string config_path):
config_path_(config_path),options_(config_path_){
    
    BuildEnvironment(options_.x_min_,options_.y_min_,options_.z_min_,
                        options_.x_scale_,options_.y_scale_,options_.z_scale_,
                        options_.x_interval_,options_.y_interval_,options_.z_interval_,
                        landmarks_);
    LandmarkToVec3d(landmarks_,landmarks_v3d_);
}

bool EnvironmentBuilder::BuildEnvironment(double x_min,double y_min,double z_min ,
                                          double x_scale, double y_scale, double z_scale ,
                                          double x_interval,double y_interval,double z_interval,
                                          std::vector<Landmark,Eigen::aligned_allocator<Landmark>> &landmark_array)
{
    if(x_scale<0||y_scale<0 || z_scale<0)
    {
        std::cout<<" the scale should be positive"<<std::endl;
        return false;
    }
    unsigned int count =0;
    landmark_array.clear();
    landmark_array.reserve(static_cast<size_t>(std::ceil(x_scale/x_interval))*
                           static_cast<size_t>(std::ceil(y_scale/y_interval))*
                           static_cast<size_t>(std::ceil(z_scale/z_interval)));
    for(double x_cur = 0.0;x_cur<=x_scale;x_cur+=x_interval)
    {
        for(double y_cur = 0.0;y_cur<=y_scale;y_cur+=y_interval)
        {
            for(double z_cur=0.0;z_cur<=z_scale;z_cur+=z_interval)
            {
                Landmark tmp_lk;
                tmp_lk.position_ = Eigen::Vector3d(x_cur+x_min,y_cur+y_min,z_cur+z_min);
                tmp_lk.label_ = count;
                landmark_array.push_back(tmp_lk);
                count++;
            }
        }
    }

    return true;
}





_E_SLAM_DEMO_
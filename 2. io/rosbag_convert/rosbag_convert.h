/*
 * @Author: Liu Weilong
 * @Date: 2021-01-20 11:20:25
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-03-22 19:17:15
 * @FilePath: /3rd-test-learning/2. io/rosbag_convert/rosbag_convert.h
 * @Description: 
 *               转换函数 1. structure_convert 从一个rosbag 里面读取 然后写入另外一个 rosbag
 *                       2. 
 * 
 * 
 */

#include <iostream>
#include <vector>
#include <string>

#include "rosbag/bag.h"
#include "rosbag/view.h"
#include "sensor_msgs/MultiEchoLaserScan.h"
#include "sensor_msgs/LaserScan.h"
#include "sensor_msgs/Imu.h"
#include "sensor_msgs/Image.h"

#include "cv_bridge/cv_bridge.h"
#include "opencv2/highgui/highgui.hpp"

using namespace std;

    void structure_convert(const string & rosbag_input_path, const string & rosbag_output_path)
    {
        rosbag::Bag bag_input,bag_output;
        bag_input.open(rosbag_input_path.c_str(),
        rosbag::bagmode::Read);
        bag_output.open(rosbag_output_path.c_str(),rosbag::bagmode::Write);

        std::vector<std::string> topics{"horizontal_laser_2d","imu"};

        rosbag::View view(bag_input,rosbag::TopicQuery(topics));

        int a =0;

        uint32_t seq = 0;
        // 以下内容主要是为了数值转换
        for(auto element: view){
   
            sensor_msgs::MultiEchoLaserScan::ConstPtr mlaserElement = element.instantiate<sensor_msgs::MultiEchoLaserScan>();
            sensor_msgs::Imu::ConstPtr imuElement = element.instantiate<sensor_msgs::Imu>();
            std::cout<<a<<std::endl;
            a++;

            if(mlaserElement != NULL){  

                // for(const sensor_msgs::LaserEcho & element:mlaserElement->ranges)
                // {
                //     std::cout<<element.echoes.front()<<"  ";
                // }
                // std::cout<<std::endl;

                sensor_msgs::LaserScan laser_t ;
                laser_t.header = mlaserElement->header;
                laser_t.header.seq = seq;
                laser_t.angle_min = mlaserElement->angle_min;
                laser_t.angle_max = mlaserElement->angle_max;
                laser_t.angle_increment = mlaserElement->angle_increment;
                laser_t.range_max = mlaserElement->range_max;
                laser_t.range_min = mlaserElement->range_min;
                laser_t.scan_time = mlaserElement->scan_time;

                seq++;

                laser_t.ranges.resize(mlaserElement->ranges.size());
                auto iter = laser_t.ranges.begin();
                std::cout<<mlaserElement->ranges.size()<<std::endl;
                int count_ =0;
                for(auto echo: mlaserElement->ranges){
                    if(echo.echoes.size()==0)
                    continue;
                    *iter = echo.echoes.front();
                    iter++;
                }
                
                laser_t.intensities.resize(mlaserElement->intensities.size());
                auto iter_ = laser_t.intensities.begin();
                for(auto echo: mlaserElement->intensities){
                    if(echo.echoes.size()==0)
                    continue;
                    *iter_ = echo.echoes.front();
                    iter_++;
                }

                bag_output.write("horizontal_laser_2d",ros::Time(mlaserElement->header.stamp),laser_t);
            }
                
            if(imuElement != NULL){
                bag_output.write("imu",ros::Time(imuElement->header.stamp),*imuElement);
            }
        }
        bag_output.close();
        bag_input.close();
    }  


    // save_path 需要内部 含有 cam0/ cam1/ cam0.txt cam1.txt
    void LoadImgAndSaveInFile(const string & rosbag_input_path, const string & save_path)
    {
        rosbag::Bag bag_input,bag_output;
        bag_input.open(rosbag_input_path.c_str(),
        rosbag::bagmode::Read);

        std::vector<std::string> topics_left{"/mynteye/left/image_raw"};
        rosbag::View view_l(bag_input,rosbag::TopicQuery(topics_left));
        std::vector<std::string> topics_right{"/mynteye/right/image_raw"};
        rosbag::View view_r(bag_input,rosbag::TopicQuery(topics_right));
        
        std::vector<std::string> cam_left_image,cam_right_image;
        cam_left_image.reserve(5000);
        cam_right_image.reserve(5000);

        // sensor_msg::ImagePtr 转cv::Mat
        for(auto element: view_l){
   
            sensor_msgs::Image::ConstPtr mLeftImagePtr = element.instantiate<sensor_msgs::Image>();
            auto img = cv_bridge::toCvCopy(mLeftImagePtr,"mono8"); 
            string line_0 = "000000000";
            string nsec_str = to_string(mLeftImagePtr->header.stamp.nsec);
            int begin = line_0.length()-nsec_str.length();
            line_0.replace(begin,nsec_str.length(),nsec_str);   
            // cout<<"show the time "<< "nsec:" <<mLeftImagePtr->header.stamp.nsec<<"     sec:"<<mLeftImagePtr->header.stamp.sec<<endl; 
            const string name = to_string(mLeftImagePtr->header.stamp.sec) + line_0 +".png";
            cam_left_image.push_back(name);
            cv::imwrite(save_path+"/cam0/"+name,img->image);
        }

        for(auto element: view_r){
   
            sensor_msgs::Image::ConstPtr mRightImagePtr = element.instantiate<sensor_msgs::Image>();
            auto img = cv_bridge::toCvCopy(mRightImagePtr,"mono8"); 
            // cout<<"show the time "<< "nsec:" <<mLeftImagePtr->header.stamp.nsec<<"     sec:"<<mLeftImagePtr->header.stamp.sec<<endl; 
            string line_0 = "000000000";
            string nsec_str = to_string(mRightImagePtr->header.stamp.nsec);
            int begin = line_0.length()-nsec_str.length();
            line_0.replace(begin,nsec_str.length(),nsec_str);    
            
            
            const string name = to_string(mRightImagePtr->header.stamp.sec) + line_0 +".png";
            cam_right_image.push_back(name);
            cv::imwrite(save_path+"/cam1/"+name,img->image);
        }
        
        ofstream fout(save_path+"/cam0.txt",ios_base::out);
        for(auto & e:cam_left_image)
        {
            fout<<e<<endl;
        }
        fout.close();
        fout.open(save_path+"/cam1.txt",ios_base::out);
        for(auto & e:cam_right_image)
        {
            fout<<e<<endl;
        }
        fout.close();

        bag_output.close();  
    }


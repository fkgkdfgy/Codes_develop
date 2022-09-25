/*
 * @Author: Liu Weilong
 * @Date: 2021-03-05 08:00:41
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-03-24 17:23:22
 * @Description: 
 */
#include "Direct_tracker_with_zero_mean.h"
#include "opencv2/opencv.hpp"
#include "common.h"

using namespace std;

std::string left_file,disparity_file,target_file;
std::string config;

std::string title="./picture/00000",end_str = ".png";

int main()
{
    left_file = "./picture/left.png";
    disparity_file = "./picture/disparity.png";
    target_file ="./picture/000001.png";
    config = "./config/config.yaml";

    cv::Mat left_img = cv::imread(left_file,0);
    cv::Mat disparity_img = cv::imread(disparity_file,0);
    cv::Mat target_img = cv::imread(target_file,0);

    int nPoints,boarder;
    float baseline,fx;

    cv::RNG rng;
    cv::FileStorage fin(config.c_str(),CV_STORAGE_READ);
    nPoints = (int)fin["DirectTracker.nPoints"];
    boarder = (int)fin["DirectTracker.boarder"];
    baseline = (double)fin["DirectTracker.Camera.baseline"];
    fx = (double)fin["DirectTracker.Camera.fx"];


    cout<<"DirectTracker.nPoints: " <<nPoints<<endl;
    cout<<"DirectTracker.boarder: " <<boarder<<endl;
    cout<<"DirectTracker.Camera.baseline: "<<baseline<<endl;
    cout<<"DirectTracker.Camera.fx: "<<fx<<endl;

    std::vector<Eigen::Vector3d,Eigen::aligned_allocator<Eigen::Vector3d>> p3d_ref;
    p3d_ref.reserve(nPoints);

    DirectTracker dm_tracker(config,false);
    const CameraInstrinc * ci = dm_tracker.GetCamera();


    std::vector<cv::Point2f> corners;
    std::vector<cv::Point2f> source;
    cv::goodFeaturesToTrack(left_img,corners,500,0.1,30);
    
    // 得到三维点
    for(int i =0;i<corners.size();i++)
    {
        int x = corners[i].x;
        int y = corners[i].y;
        int disparity = disparity_img.at<uchar>(y,x);
        double depth = fx*baseline/float(disparity);
        if(depth<0)
        continue;
        
        Eigen::Vector3d temp = ci->p2c(Eigen::Vector2d(x,y)) * depth;
        p3d_ref.push_back(temp);    
        source.push_back(corners[i]);    
    }
    
    // cv::Mat left_img_rgb;
    // cv::cvtColor(left_img,left_img_rgb,CV_GRAY2RGB);
    
    // DrawCorners(left_img_rgb,corners);
    
    // cv::imshow("left_img",left_img_rgb);
    // cv::waitKey(0);

    Eigen::Matrix<double,6,1> result = Eigen::Matrix<double,6,1>::Zero();

    cv::Mat origin_rbg;
    cv::cvtColor(left_img,origin_rbg,CV_GRAY2RGB);
    DrawCorners(origin_rbg,source);
    cv::imshow("origin",origin_rbg);
    
    std::vector<bool> good_pts;
    good_pts.resize(p3d_ref.size(),true);
    for(int i=1;i<6;i++)
    {
        target_img = cv::imread(title+to_string(i)+end_str,0);
        dm_tracker.m = 0.;
        dm_tracker.SetLastFramePoints(p3d_ref,good_pts);
        dm_tracker.SetLastFramePose(Eigen::Matrix<double,6,1>::Zero());
        dm_tracker.SetInput(left_img);

        dm_tracker.SetPredict(result);
        dm_tracker.SetTarget(target_img);
        
        dm_tracker.Run();
        result = dm_tracker.GetResult();
        good_pts = dm_tracker.GetGoodPts();

        cout<<"the translation is "<< Sophus::SE3d::exp(result).translation().transpose()<<endl;

        cout<<"the result m is "<<dm_tracker.m<<endl;
        
        const CameraInstrinc *cm = dm_tracker.GetCamera();
        Sophus::SE3d se3_cur = Sophus::SE3d::exp(result);
        std::vector<cv::Point2f> p2_array;

        for(const auto & p: p3d_ref)
        {
            auto temp_pt = se3_cur*p;
            auto temp_px = cm->c2p(temp_pt);
            p2_array.push_back(cv::Point2f(temp_px.x(),temp_px.y()));
        }

        cv::Mat target_rgb= target_img.clone();
        cv::cvtColor(target_rgb,target_rgb,CV_GRAY2RGB);
        cout<<"target_rbg,type"<<target_rgb.type()<<endl;
        
        DrawForMatchResult(target_rgb,source,p2_array);
        cv::imshow("result",target_rgb);
        cv::waitKey(0);

        
    }



    cout<<"the result is "<<dm_tracker.GetResult().matrix()<<endl;



}

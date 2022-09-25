/*
 * @Author: Liu Weilong
 * @Date: 2021-02-27 23:18:39
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-03-03 19:01:07
 * @Description: 
 */

#include "tracker_base.h"
#include "opencv2/video/tracking.hpp"
#include "common.h"

class OpencvTracker:public TrackerBase
{
    public:

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    using OpencvCorners = std::vector<cv::Point2f>;

    OpencvTracker(const std::string config):TrackerBase(config){}
    virtual void Run() override;
    virtual void Impl() override;
    OpencvCorners cv_pre_corners_,cv_cur_corners_;

};

void OpencvTracker::Run() 
{
    eigen2cv(pre_corners_,cv_pre_corners_);
    auto start = chrono::high_resolution_clock::now();
    Impl();
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> diff = end - start;
    cout<<"Total: "<< diff.count()<<" s"<<endl;
    cv2eigen(cv_cur_corners_,cur_corners_);
}

void OpencvTracker::Impl()
{
    std::vector<uchar> status;
    std::vector<float> error;
    cv::calcOpticalFlowPyrLK(pre_img_,cur_img_,cv_pre_corners_,cv_cur_corners_,status,error,
    cv::Size(options_ptr_->template_,options_ptr_->template_),options_ptr_->level_);
}





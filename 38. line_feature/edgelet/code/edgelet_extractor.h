/*
 * @Author: Liu Weilong
 * @Date: 2021-03-05 16:32:36
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-03-08 13:57:52
 * @FilePath: /3rd-test-learning/38. line_feature/edgelet/code/edgelet_extractor.h
 * @Description: 
 */

#include <iostream>
#include <vector>
#include <string>
#include <chrono>

#include "Eigen/Eigen"
#include "opencv2/core/core.hpp"
#include "opencv2/opencv.hpp"

using namespace std;

class Edgelet
{
    public:
    
    Edgelet(float mag, const Eigen::Vector2f & px,
            const Eigen::Vector2f & g, const Eigen::Vector3f & pos = Eigen::Vector3f::Zero()):
            grad_mag_(mag),grad_(g),px_(px),pos_(pos){}
    
    float grad_mag_;
    Eigen::Vector2f grad_;
    Eigen::Vector2f px_;
    Eigen::Vector3f pos_;
};

// class EdgeletExtractorOptions
// {
//     public:
// };

// class EdgeletExtractor
// {
//     public:
    
//     void SetTarget(const cv::Mat target){target_img_ = target;}
//     void Run();
//     virtual void Impl();

//     EdgeletExtractorOptions * options_ptr_;
//     cv::Mat target_img_; 
//     std::vector<Edgelet> result_edgelets_;
// };

// void EdgeletExtractor::Run()
// {
//     auto start = chrono::high_resolution_clock::now();
//     Impl();
//     auto end = chrono::high_resolution_clock::now();
//     chrono::duration<double> diff = end - start;
//     cout<<"Edgelet Extraction Cost : "<< diff.count()<<" s"<<endl;
// }

// void EdgeletExtractor::Impl()
// {
//     cv::Mat img = target_img_.clone();
//     cv::Mat gradx = cv::Mat::zeros( target_img_.rows, target_img_.cols, CV_32F);
//     cv::Mat grady = cv::Mat::zeros(target_img_.rows, target_img_.cols, CV_32F);
//     cv::Mat mag =  cv::Mat::zeros(target_img_.rows, target_img_.cols, CV_32F);

//     cv::GaussianBlur(target_img_,img,cv::Size(3,3),0,0);
//     cv::Scharr(img, gradx, CV_32F, 1, 0, 1/32.0);
//     cv::Scharr(img, grady, CV_32F, 0, 1, 1/32.0);
//     cv::magnitude(gradx,grady,mag);

//     cv::Mat canny;
//     cv::Canny(,)

// }

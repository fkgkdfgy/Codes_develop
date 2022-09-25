/*
 * @Author: Liu Weilong
 * @Date: 2021-03-05 16:32:55
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-07-06 15:26:51
 * @FilePath: /3rd-test-learning/38. line_feature/edgelet/code/test_edgelet.cc
 * @Description: 
 * 
 * PC端 752 480 30ms 
 * 
 * 
 */
#include <iostream>
#include <chrono>

#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/video/tracking.hpp"
#include "eigen3/Eigen/Eigen"
#include "edgelet_extractor.h"

using namespace std;
using namespace Eigen;

std::string img_path;

int main()
{
    img_path = "/home/lwl/workspace/3rd-test-learning/38. line_feature/source_pic/1.png";
    
    auto start = chrono::high_resolution_clock::now();
    

    cv::Mat img = cv::imread(img_path.c_str(),0);
    cv::Mat img2 = img.clone();
    
    cv::Mat gradx = cv::Mat::zeros( img.rows, img.cols, CV_32F);
    cv::Mat grady = cv::Mat::zeros(img.rows, img.cols, CV_32F);
    cv::Mat mag =  cv::Mat::zeros(img.rows, img.cols, CV_32F);

    cv::GaussianBlur(img,img2,cv::Size(3,3),0,0);
    cv::Scharr(img2, gradx, CV_32F, 1, 0, 1/32.0);
    cv::Scharr(img2, grady, CV_32F, 0, 1, 1/32.0);
    cv::magnitude(gradx,grady,mag);

    int cell_size = 25;    
    int cell_x = std::ceil(double(img.cols))/cell_size;
    int cell_y = std::ceil(double(img.rows))/cell_size;


    int x_start = 0, y_start=0;

    std::vector<Edgelet> edge_list;

    for(int i=0;i<cell_x;i++)
    {
        for(int j=0;j<cell_y;j++)
        {

            float max_grad = 0;
            int maxgrad_x = 0;
            int maxgrad_y = 0;
            float gx = 0;
            float gy = 0;

            cv::Mat canny;
            cv::Rect roi(x_start,y_start,cell_size,cell_size);
            cv::Canny(img(roi) , canny , 30,50);
            
            float max_grad_2 = 0;
            int maxgrad_x2 = 0;
            int maxgrad_y2 = 0;


            // 每个格子最大梯度canny 点 进行提取
            for (int i=0;i<cell_size;i++ )
            for(int j=0;j<cell_size;j++)
            {
                if(canny.ptr<uchar>(i)[j] == 0) continue;
                float temp = mag.ptr<float>(y_start + i)[x_start+j] ;

                if( temp> max_grad )
                {
                maxgrad_x = x_start+j;
                maxgrad_y = y_start + i;
                max_grad = temp;
                gx = gradx.ptr<float>(maxgrad_y)[maxgrad_x] ;
                gy = grady.ptr<float>(maxgrad_y)[maxgrad_x] ;
                }
            }
            
            
            
            int edge_threshold = 4;
            {

                Vector2d g = Vector2d(gx,gy);
                g.normalize();

                if( max_grad > edge_threshold)
                {
                    Edgelet e(max_grad,Vector2f(maxgrad_x, maxgrad_y),g.cast<float>());
                    edge_list.push_back(e);
                    //fts.push_back(new Feature(frame, Vector2d(maxgrad_x, maxgrad_y),g, 0));   // edge
                }
                // else if( max_grad_2 >0.0)
                // {
                //     Pt p(max_grad_2,Vector2d(maxgrad_x2,maxgrad_y2));
                //     maxgrad_list.push_back(p);
                // //     fts.push_back(new Feature(frame, Vector2d(maxgrad_x2,maxgrad_y2), 0));
                // }
            }

            y_start+=cell_size;
        }
        x_start +=cell_size;
        y_start = 0;
    }


    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> diff = end - start;
    cout<<"Total: "<< diff.count()<<" s"<<endl;

    
    cv::Mat rgb ;
    cv::cvtColor(img,rgb,CV_GRAY2RGB);
    for(auto it=edge_list.begin(); it!=edge_list.end(); ++it)
    {
        cv::line(rgb,
                 cv::Point2f(it->px_.x()+3*it->grad_.y(), it->px_.y()-3*it->grad_.x()),
                 cv::Point2f(it->px_.x()-3*it->grad_.y(), it->px_.y()+3*it->grad_.x()),
                 cv::Scalar(0,0,255), 2);
    }

    cv::imshow("edgelet", rgb);
    cv::waitKey(0);



    





    
    // for(int i=0;i<cell_x;i++)
    //     for(int j=0;j<cell_y;j++)
    //     {
    //         cv::Rect roi(x_start,y_start,cell_size,cell_size);
    //         cv::Mat canny;
    //         cv::Canny(img(roi),canny,30,50);
    //     }
    
    // auto end = chrono::high_resolution_clock::now();
    // chrono::duration<double> diff = end - start;
    // cout<<"Edgelet Preparation Cost : "<< diff.count()<<" s"<<endl;

    // cv::imshow("origin img",img);
    // cv::imshow("canny",canny);
    // cv::waitKey(0);

}
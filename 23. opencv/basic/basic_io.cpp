/*
 * @Author: Liu Weilong
 * @Date: 2020-11-11 09:01:40
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-01-09 09:42:28
 * @FilePath: /3rd-test-learning/26. opencv/basic_io.cpp
 * @Description: 测试opencv cmake 和io 进行  
 */

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <string>
using namespace std;

int main()
{   
    std::string img_path1,img_path2;
    img_path1 = "/home/lwl/workspace/3rd-test-learning/26. opencv/1.png";
    img_path2 = "/home/lwl/workspace/3rd-test-learning/26. opencv/2.png";
    cv::Mat img1 = cv::imread(img_path1);
    cv::Mat img2 = cv::imread(img_path2);
    
    cout<<"show the rows"<<img1.rows<<endl;
    cout<<"show the cols"<<img1.cols<<endl;


    cv::imshow("aaa",img1);
    cv::waitKey(0);
    while(1)
    {

        // 原来只要名字一样可以不断的显示
        cv::waitKey(10);
        cv::imshow("AAA",img1);
        cv::waitKey(10);
        cv::imshow("AAA",img2);
    }

    
    return 0;
}
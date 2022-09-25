/*
 * @Author: Liu Weilong
 * @Date: 2021-01-20 14:16:49
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-01-20 14:38:42
 * @FilePath: /3rd-test-learning/2. io/image_io/data_loader_test.cpp
 * @Description: 
 */
#include <iostream>
#include <string>

#include "data_loader.h"
#include "opencv2/highgui/highgui.hpp"
using namespace std;
int main()
{
    DataLoader dl("../all_path.yaml");

    cv::Mat img_l,img_r;
    
    while(dl.LoadImg(img_l,img_r))
    {
        cv::imshow("Left Camera",img_l);
        cv::imshow("Right Camera",img_r);
        cv::waitKey(10);
    }
    
    
}
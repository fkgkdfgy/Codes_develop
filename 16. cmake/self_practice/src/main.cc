/*
 * @Author: Liu Weilong
 * @Date: 2020-10-27 09:11:35
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-01-14 09:35:06
 * @FilePath: /3rd-test-learning/16. cmake/src/main.cc
 * @Description: 
 */
#include <iostream>
#include "foo.h"
#include "opencv2/core/core.hpp"

using namespace std;
int main()
{
    cv::Mat temp(6,6,CV_32F);
    cout<<temp<<endl;
    foo();
}
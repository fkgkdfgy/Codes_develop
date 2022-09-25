/*
 * @Author: Liu Weilong
 * @Date: 2021-05-01 22:28:39
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-05-02 13:44:49
 * @Description: 
 */
#include <iostream>
#include <string>
#include <vector>

#include "data_loader_func.h"
#include "opencv2/core/core.hpp"
#include "opencv2/opencv.hpp"

using namespace std;

int length = 0;

bool sortDJI(const std::string & name1, const std::string & name2)
{
    long time1 = atol(name1.substr(11+length,13).c_str());
    long time2 = atol(name2.substr(11+length,13).c_str());
    return time1<time2;
}

int main (int argc, char ** argv)
{
    std::vector<std::string> filenames;
    std::string dir = argv[1];
    get_filenames(dir,filenames);

    length = dir.length()+1;
    // for(int i =0;i<10;i++)
    // cout<<filenames[i]<<endl;
    
    std::sort(filenames.begin(),filenames.end(),sortDJI);


    for(int i =0;i<10;i++)
    cout<<filenames[i]<<endl;
    
    cv::Mat img = cv::imread(filenames[0]);
    cout<<"col :"<<img.cols<<endl<<"rows:"<<img.rows<<endl;
    
}


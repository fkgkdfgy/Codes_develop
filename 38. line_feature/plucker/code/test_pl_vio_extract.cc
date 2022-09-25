/*
 * @Author: Liu Weilong
 * @Date: 2021-03-08 16:23:01
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-03-08 18:15:04
 * @FilePath: /3rd-test-learning/38. line_feature/plucker/code/test_pl_vio_extract.cc
 * @Description: 
 * 
 * 
 * 存在的问题:
 * 1. 旋转匹配，导致的错误匹配   最后发现是忘记筛选distance了
 * 2. 存在太多误匹配，怀疑是API使用的问题 最后发现是忘记筛选distance了
 * 3. 存在线飞起来的情况 还没有查清楚
 * 
 */
#include "common.h"

using namespace std;
using namespace cv;
using namespace cv::line_descriptor;

#define Tic {auto start = chrono::high_resolution_clock::now();
#define Toc auto end = chrono::high_resolution_clock::now();\
    chrono::duration<double> diff = end - start;\
    cout<<"Total: "<< diff.count()<<" s"<<endl;}

std::string img1_path,img2_path;

int main()
{
    img1_path="../../../source_pic/1.png";
    img2_path="../../../source_pic/2.png";

    cv::Mat img1,img2;
    img1 = imread(img1_path,0);
    img2 = imread(img2_path,0);
    resize(img1,img1,Size(img1.cols * 0.5,img1.rows*0.5));
    resize(img2,img2,Size(img2.cols * 0.5,img2.rows*0.5));

    std::vector<KeyLine> lsd1, keylsd1;
    std::vector<KeyLine> lsd2, keylsd2;
    Ptr<LSDDetector> lsd_;
    Tic
        lsd_ = cv::line_descriptor::LSDDetector::createLSDDetector();
        lsd_->detect( img1, lsd1, 2, 2 );     
        lsd_->detect( img2, lsd2, 2, 2 );     
    Toc

    Mat keylbd_descr1,lbd_descr1;
    Mat keylbd_descr2,lbd_descr2;

    Tic
        Ptr<BinaryDescriptor> bd_ = BinaryDescriptor::createBinaryDescriptor();
        bd_->compute( img1, lsd1, lbd_descr1 );
        bd_->compute( img2, lsd2, lbd_descr2 );
    Toc

    for ( int i = 0; i < (int) lsd1.size(); i++ )
    {
        if( lsd1[i].octave == 0 && lsd1[i].lineLength >= 40)
        {
            keylsd1.push_back( lsd1[i] );
            keylbd_descr1.push_back( lbd_descr1.row( i ) );
        }
    }

    for ( int i = 0; i < (int) lsd2.size(); i++ )
    {
        if( lsd2[i].octave == 0 && lsd2[i].lineLength >= 40)
        {
            keylsd2.push_back( lsd2[i] );
            keylbd_descr2.push_back( lbd_descr2.row( i ) );
        }
    }


    std::vector<DMatch> lsd_matches;
    
    Tic
    Ptr<BinaryDescriptorMatcher> bdm_;
    bdm_ = BinaryDescriptorMatcher::createBinaryDescriptorMatcher();
    bdm_->match(keylbd_descr1, keylbd_descr2, lsd_matches);
    Toc

    cv::Mat rgb1,rgb2;
    cv::Mat mrbg1,mrbg2;

    std::vector<DMatch> want;

    for(auto & mt:lsd_matches)
    {
        if(mt.distance<8) want.push_back(mt);
    }
    

    
    cvtColor(img1,rgb1,CV_GRAY2RGB);
    cvtColor(img2,rgb2,CV_GRAY2RGB);
    cvtColor(img1,mrbg1,CV_GRAY2RGB);
    cvtColor(img2,mrbg2,CV_GRAY2RGB);

    mrbg2 = mrbg1.clone();
    

    DrawLine(rgb1,keylsd1);
    DrawLine(rgb2,keylsd2);
    DrawMatch(mrbg1,mrbg2,keylsd1,keylsd2,want);
    imshow("lines1",rgb1);
    imshow("lines2",rgb2);
    imshow("R1",mrbg1);
    imshow("R2",mrbg2);

    waitKey(0);

}
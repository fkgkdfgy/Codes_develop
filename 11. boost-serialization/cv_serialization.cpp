/*
 * @Author: Liu Weilong
 * @Date: 2021-01-22 13:25:27
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-01-24 16:45:31
 * @FilePath: /3rd-test-learning/11. boost-serialization/cv_serialization.cpp
 * @Description: 
 *              主要用于 vector<Mat> vector<Point_<T>> Mat 进行序列化
 */
#include <iostream>
#include <fstream>
#include <vector>

#include "opencv2/core/core.hpp"

#include "boost/serialization/vector.hpp"

#include "boost/archive/text_iarchive.hpp"
#include "boost/archive/text_oarchive.hpp"
#include "boost/archive/binary_iarchive.hpp"
#include "boost/archive/binary_oarchive.hpp"

using namespace std;

namespace boost {
namespace serialization {

template<class Archive>
void serialize(Archive &ar, cv::Mat& mat, const unsigned int)
{
    int cols, rows, type;
    bool continuous;

    if (Archive::is_saving::value) {
        cols = mat.cols; rows = mat.rows; type = mat.type();
        continuous = mat.isContinuous();
    }

    ar & cols & rows & type & continuous;

    if (Archive::is_loading::value)
        mat.create(rows, cols, type);

    if (continuous) {
        const unsigned int data_size = rows * cols * mat.elemSize();
        ar & boost::serialization::make_array(mat.ptr(), data_size);
    }
    else {
        const unsigned int row_size = cols*mat.elemSize();
        for (int i = 0; i < rows; i++) {
            ar & boost::serialization::make_array(mat.ptr(i), row_size);
        }
    }
}

template<class Archive,typename T>
void serialize(Archive &ar, cv::Point_<T>& point, const unsigned int)
{
    ar & point.x;
    ar & point.y;
}

}
}


class Frame
{
    public:
    friend class boost::serialization::access;

    template<typename Archive>
    void serialize(Archive &ar, const unsigned int)
    {
        ar & imgs_;
        ar & pts_;
        ar & ref_;
    }
    
    vector<cv::Mat> imgs_; 
    vector<cv::Point2f> pts_;
    cv::Mat ref_;
};

void show(Frame & f)
{
    for(auto img:f.imgs_)
    {
        cout<<"the idx of img is "<< img.at<float>(0,0)<<endl;
    }

    for(auto pt:f.pts_)
    {
        cout<<"the point is "<<pt.x<<endl;
    }

    if(!f.ref_.empty())
    cout<<"the ref is "<<endl<<f.ref_.rowRange(0,2).colRange(0,2)<<endl;
}


int main()
{
    // 准备存储的数据
    Frame f1;

    // 数据填充
    for(int i=0;i<20;i++)
    {
        f1.imgs_.push_back(cv::Mat::eye(5,5,CV_32F)*float(i));
        f1.pts_.push_back(cv::Point2f(float(i),float(i)));
    }   

    f1.ref_ = cv::Mat::zeros(30,30,CV_32F);
    
    std::string filename_text = "/home/sunny/workspace_private/3rd-test-learning/11. boost-serialization/build/file_text";
    std::string filename_bin = "/home/sunny/workspace_private/3rd-test-learning/11. boost-serialization/build/file_bin";
    std::ofstream ofs_t(filename_text.c_str());
    std::ofstream ofs_b(filename_bin.c_str());
    {
        boost::archive::text_oarchive oa(ofs_t);
        oa<<f1; 
    }
    
    {
        boost::archive::binary_oarchive oa(ofs_b);
        oa<<f1;
    }
    
    Frame f2;
    {
        std::ifstream ifs(filename_text.c_str());
        boost::archive::text_iarchive ia(ifs);
        ia>>f2;
    }

    show(f2);
    Frame f3;
    {
        std::ifstream ifs(filename_bin.c_str());
        boost::archive::binary_iarchive ia(ifs);
        ia>>f3;
    }
    show(f3);
}
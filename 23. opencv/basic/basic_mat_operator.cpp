/*
 * @Author: Liu Weilong
 * @Date: 2020-12-24 14:56:20
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-03-18 14:01:20
 * @FilePath: /3rd-test-learning/23. opencv/basic/basic_mat_operator.cpp
 * @Description: 
 *              主要是为了测试和学习 opencv Mat 的操作
 *              包含以下内容:
 *              1. 矩阵读取
 *              2. 矩阵块处理
 *              3. 先拷贝
 *              4. 矩阵加减乘除 取逆  经过测试都可以进行使用
 *              5. 存储类型    和 4  一起测试过了
 *              6. channel 和 reshape
 *              7. 行赋值 列赋值 vector赋值
 */

#include <iostream>
#include <vector>

#include "opencv2/core/core.hpp"
using namespace std;

void showMat(cv::Mat WHAT)
{
    for(int row = 0;row<WHAT.rows;row++)
    {
        for(int col = 0;col<WHAT.cols;col++)
        {cout<< WHAT.at<double>(row,col)<<" ";}
        cout<<endl;
    }
}
void showMatMultiChannel(cv::Mat WHAT)
{
    for(int row =0 ;row<WHAT.rows;row++)
    {
        double * tmp_ptr = WHAT.ptr<double>(row);
        for(int i=0;i<WHAT.cols*WHAT.channels();i++)
        cout<<tmp_ptr[i]<<" ";
        cout<<endl;
    }
}
void showData(cv::Mat WHAT)
{
    double * tmp_ptr = (double *)WHAT.data;
    for(int i=0;i<WHAT.rows*WHAT.cols*WHAT.channels();i++)
    {
        cout<<tmp_ptr[i]<<" ";
    }
}
int main()
{
    // cv::FileStorage fin("../basic/mat_storage.yaml",CV_STORAGE_READ);
// =============================== 1. 矩阵读取 ====================================
    // cv::Mat WHAT;
    // fin["WHAT"]>>WHAT;
    // showMat(WHAT);
// =============================== 2. 矩阵部分数值处理 =============================
    // WHAT(cv::Range(0,2),cv::Range(0,2))*=2;
    // showMat(WHAT);
// =============================== 3. 矩阵浅拷贝 ================================== 
    // cv::Mat what = WHAT(cv::Range(0,2),cv::Range(0,2));
    // what*=2;
    // showMat(WHAT);
// =============================== 4. 矩阵计算 =======================================
    // cv::Mat element(3,1,WHAT.type());
    // data 直接赋值失败 原因是这里转换的type 没有使用double * 而是 float * 数据上就一定会出现混乱
    // float* temp_ptr = (float*)element.data;
    // temp_ptr[0] = 1.0; temp_ptr[1]=1.0; temp_ptr[2]=1.0;
    // element = (cv::Mat_<double>(3, 1) <<1.0,1.0,1.0);
    // showMat(element);
    // cout<<"the channel of element is "<< element.channels()<<endl;
    // element = element + element;
    // showMat(element);
    // element = WHAT * element;
    // showMat(element);
    // cout<<WHAT.inv()<<endl;
// ============================= 6. channel 和 reshape ============================
    // 依旧是一个浅拷贝只是换了一个表达方式
    // cv::Mat new_WHAT = WHAT.reshape(3,0);
    // cout<<"the new channel of WHAT is "<< new_WHAT.channels()<<endl;
    // cout<<"the origin Mat addr is "<<reinterpret_cast<unsigned long>(WHAT.data)<<endl;;
    // cout<<"the new Mat addr is "<<reinterpret_cast<unsigned long>(new_WHAT.data)<<endl;
    // showMat(new_WHAT);
    // showMatMultiChannel(new_WHAT);
    // showData(new_WHAT);


// ============================= 7. 线和列的拷贝 =============================

    cv::Mat test = cv::Mat::eye(3,3,CV_32F);
    cv::Mat test1 = cv::Mat::eye(3,3,CV_32F)*2;
    vector<cv::Mat> a;
    // 没有进行赋值
    test.row(1) =  test1.row(1);
    test.row(1) = test1.row(1).clone();
    cout<<test<<endl;

    // 进行了赋值
    test1.row(1).copyTo(test.row(1));
    
    cout<<test<<endl;
    a.emplace_back(test.row(1).clone());
    cout<<a[0]<<endl;
    test.row(1)= test.row(1)*2;
    cout<<test<<endl;
    cout<<a[0]<<endl;
    return 0;
}
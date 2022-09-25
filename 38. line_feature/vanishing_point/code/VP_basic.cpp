/*
 * @Author: Liu Weilong
 * @Date: 2021-03-25 18:53:46
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-07-06 12:20:20
 * @FilePath: /3rd-test-learning/38. line_feature/vanishing_point/code/VP_basic.cpp
 * @Description: 
 */
#include "environment_builder.h"

Box b1 = Box::create(Eigen::Vector3d(0,0,0+0.3),0.2);
Box b2 = Box::create(Eigen::Vector3d(0,0,0+0.5),0.2,cv::Scalar(0,255,0));
Box b3 = Box::create(Eigen::Vector3d(0,0,0+0.7),0.2,cv::Scalar(0,0,255));

BoxDisplayer bd;
Camera cm;
Eigen::Matrix<double,6,1> se3;
int x=0.,y=0.,z=0.,r=0.,p=0.,yaw=0.;

void Init()
{
    bd.vboxobs.clear();

    double x_ = float(x)/100;
    double y_ = 30/100;
    double z_ = float(z)/100;
    
    double r_ = float(r)/300;
    double p_ = float(p)/300;
    double yaw_ = float(yaw)/300;


    se3<<x_,y_,z_,r_,p_,yaw_;
    cm.T_w_c = Sophus::SE3d::exp(se3);
    bd.vboxobs.push_back(b1.generateObs(&cm));
    bd.vboxobs.push_back(b2.generateObs(&cm));
    bd.vboxobs.push_back(b3.generateObs(&cm));
}

int main()
{
    Init();
}
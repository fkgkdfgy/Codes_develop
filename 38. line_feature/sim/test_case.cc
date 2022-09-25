/*
 * @Author: Liu Weilong
 * @Date: 2021-03-25 18:53:46
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-05-22 23:13:53
 * @FilePath: /Codes/38. line_feature/vanishing_point/code/test_case.cpp
 * @Description: 
 * 
 * 这个test_case 主要是为了测试 使用灭点进行定位的正确性:
 * 主要内容：
 * 1. OpenCVPoseAdjustAndEstimate() 调整bar来调整相机位姿
 * 2. VanishingLineTest()  检测在曼哈顿假设下Vanishing Line 变化和估计差别是不是很大 失败
 * 3. VanishingRansac1Line()
 * 4. 2021.3.30 今天添加了重力方面的检测，这个重力方向的测试，也说明了一个问题,就是在 raw pitch 如何变化，raw pitch 是多少 yaw 对应的 VP 就是多少
 *    yaw的变化，不会对yaw 对应的VP 产生影响
 *    这也是为什么IMU 的重力加速度可以很好的用于 VPs 的clustering 因为从IMU 重力加速度中得到的yaw pitch + 一个随便的yaw 就可以正确估计出yaw 对应VP的参数
 * 5. 但是好像还有一些别的问题需要解决
 * 6. RefineVP 用于测试SVD求解非零解
 * 
 * tips:
 * 1. vp 的估计存在方向问题 用防止突变的方法解决了这个问题
 * 
 * 
 */
#include "environment_builder.h"
#include "VPDetection.h"

Box b1 = Box::create(Eigen::Vector3d(0,0,0+0.3),0.2);
Box b2 = Box::create(Eigen::Vector3d(0,0,0+0.5),0.2,cv::Scalar(0,255,0));
Box b3 = Box::create(Eigen::Vector3d(0,0,0+0.7),0.2,cv::Scalar(0,0,255));
Box b4 = Box::create(Eigen::Vector3d(0,0.1,0+0.7),0.6,cv::Scalar(0,200,100));
BoxDisplayer bd;
Camera cm;
Eigen::Matrix<double,6,1> se3;
int x=0.,y=0.,z=0.,r=0.,p=0.,yaw=0.;
Eigen::Matrix3d K_inv = cm.K().inverse();
Eigen::Vector3d pre_x(1.0,0.,0.),pre_y(0.,1.0,0.),pre_z(0.,0.,1.);

void Adjust(int, void *);
void OpenCVPoseAdjustAndEstimate();
void VPsDetection();
void RefineVP();


int main()
{
    // 基础内容测试
    // se3<<0.,0.,0.,0.,0.,0.;
    // cm.T_w_c = Sophus::SE3d::exp(se3);

    // cout<<"real SO3"<<endl<<cm.T_w_c.so3().matrix().inverse()<<endl;

    // bd.vboxobs.push_back(b1.generateObs(&cm));
    // bd.vboxobs.push_back(b2.generateObs(&cm));
    // bd.vboxobs.push_back(b3.generateObs(&cm));
    // bd.vboxobs.push_back(b4.generateObs(&cm));

    // BoxObservation obs = b1.generateObs(&cm);
    // Eigen::Vector3d vl1 = K_inv*obs.toVL(obs.v2dobs_x,obs.v2dobs_y);
    // vl1.normalize();
    // Camera cm1;
    // Eigen::Matrix<double,6,1> se31;
    // se31<<1.,1.,1.,1.,1.,0.;
    // cm1.T_w_c = Sophus::SE3d::exp(se31);
    
    // BoxObservation obs1 = b1.generateObs(&cm1);
    // Eigen::Vector3d vl2 = K_inv*obs.toVL(obs1.v2dobs_x,obs1.v2dobs_y);
    // vl2.normalize();
    // cout<<"the real vl :"<<vl1.transpose()<<endl;
    // cout<<"the real vl :"<<vl2.transpose()<<endl;

    OpenCVPoseAdjustAndEstimate();

}

void OpenCVPoseAdjustAndEstimate()
{
    cv::namedWindow("ir_gray", CV_WINDOW_NORMAL);			//滑动窗口选择ir图canny阈值
    cv::createTrackbar("value1：", "ir_gray", &x,300 , Adjust);
    cv::createTrackbar("value2", "ir_gray", &y, 300, Adjust);
    cv::createTrackbar("value3：", "ir_gray", &z, 300, Adjust);
    cv::createTrackbar("value4:", "ir_gray", &r, 300, Adjust);
    cv::createTrackbar("value5：", "ir_gray", &p, 300, Adjust);
    cv::createTrackbar("value6", "ir_gray", &yaw, 300, Adjust);
    Adjust(0, 0);
    cv::waitKey(0);
}

void Adjust(int, void *)
{
    bd.vboxobs.clear();

    double x_ = float(x)/100;
    double y_ = float(y)/100;
    double z_ = float(z)/100;
    
    double r_ = float(r)/300;
    double p_ = float(p)/300;
    double yaw_ = float(yaw)/300;


    Eigen::AngleAxisd aa_x(r_,Eigen::Vector3d(1,0,0));
    Eigen::AngleAxisd aa_y(p_,Eigen::Vector3d(0,1,0));
    Eigen::AngleAxisd aa_z(yaw_,Eigen::Vector3d(0,0,1));

    cm.T_w_c.setQuaternion((aa_z*aa_y*aa_x));

    cm.T_w_c.translation() = Eigen::Vector3d(x_,y_,z_);
    



    cout<<"real SO3"<<endl<<cm.T_w_c.so3().matrix().inverse()<<endl;

    bd.vboxobs.push_back(b1.generateObs(&cm));
    bd.vboxobs.push_back(b2.generateObs(&cm));
    bd.vboxobs.push_back(b3.generateObs(&cm));
    bd.vboxobs.push_back(b4.generateObs(&cm));
    
    ImageLineEigen x_l1 = bd.vboxobs.front().v2dobs_x[0],
                   x_l2 = bd.vboxobs.front().v2dobs_x[1];
    ImageLineEigen y_l1 = bd.vboxobs.front().v2dobs_y[0],
                   y_l2 = bd.vboxobs.front().v2dobs_y[1];
    ImageLineEigen z_l1 = bd.vboxobs.front().v2dobs_z[0],
                   z_l2 = bd.vboxobs.front().v2dobs_z[1];

    // 测试性代码  start

    Eigen::Vector3d vp_x = (K_inv*getVPFromeLine(x_l1,x_l2)).normalized();
    Eigen::Vector3d vp_y = (K_inv*getVPFromeLine(y_l1,y_l2)).normalized();
    Eigen::Vector3d vp_z = (K_inv*getVPFromeLine(z_l1,z_l2)).normalized();
    
    if(vp_x.dot(pre_x)<0)
    vp_x *=-1;
    if(vp_y.dot(pre_y)<0)
    vp_y *=-1;
    if(vp_z.dot(pre_z)<0)
    vp_z *=-1;
    
    pre_x = vp_x;
    pre_y = vp_y;
    pre_z = vp_z;

    Eigen::Matrix3d est;
    est<<vp_x,vp_y,vp_x.cross(vp_y);
    cout<<"esitimate :"<<endl<<est<<endl;
    cout<<"the norm of estimate :"<<est.determinant()<<endl;
    
    // 测试性代码  end

    cv::Mat img_box = bd.showBox().clone();
    cv::Mat img_xyz = bd.showXYZ(true).clone();

    cv::imshow("box_test",img_box);
    cv::imshow("xyz_test",img_xyz);

}

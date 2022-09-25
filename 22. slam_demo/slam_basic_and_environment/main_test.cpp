/*
 * @Author: Liu Weilong
 * @Date: 2021-01-15 07:00:51
 * @Description:
 *              测试内容
 *              1. 测试颜色的转换          ColorConvert 
 *              2. 显示测试               EnvironmentShow
 *              3. 内参转换               CameraModel
 *              4. Eigen和Mat 的内存转换   EigenToMat   发现只有一行(列)或者方阵 用起来才不会有风险
 *              5. 单应矩阵解算            H_matrix     四个点不能共线，不能出现交叉关联的情况
 *              6. 基础矩阵结算            F_matrix     查看旋转退化的情况  以及 退化情况下 F 和H 的选择
 *                                                    自己写的F矩阵求解存在问题
 *                                                    opencv的findF 也存在问题 估计是点集产生的问题
 *                                                    最后发现是比较方法的问题
 *              7. 运动解算
 */

// ! 之前的错误是因为 Sophus 前面没有 include Eigen
// ! 导致Sophus 内部找不到 Eigen 的namespace
#include "Eigen/Eigen"
#include "sophus/so3.hpp"



#include "environment_builder.h"
#include "pointcloud_shower.h"
#include "frame_interface.h"
#include "common.h"
#include "camera_model.h"
#include "convert.h"


#include "opencv2/calib3d/calib3d.hpp"


using namespace slam_demo;
using namespace std;

void ColorConvert()
{
    PangolinPointCloudShower shower_;
    Eigen::Vector3i color(128,128,128);
    Eigen::Vector3i color_converted;
    uint32_t ui32;
    shower_.ColorConvert(color,ui32);
    shower_.ColorConvert(ui32,color_converted);
    cout<<"the origin color is "<< color.transpose()<<endl;
    cout<<"the converted color is "<<color_converted.transpose()<<endl;
}

void EnvironmentShow()
{
    const string config_path= "../config/Environment_H.yaml";
    EnvironmentBuilder eb(config_path);
    Eigen::Vector3i color(255,0,0);
    PangolinPointCloudShower pps;
    pps.LoadPointCloud(color,eb.GetLandmarksVec3d());
    pps.Show();
}

void CameraModel()
{
    const string config_path ="../config/EuRoC.yaml";
    CameraModelPinhole cm(config_path);
    Eigen::Vector3d temp_point(1.0,1.0,2.0);
    Eigen::Vector3d expect_point= temp_point/temp_point.z();
    std::vector<Eigen::Vector3d> vtemppts{temp_point};
    std::vector<Eigen::Vector2d> vuv;
    
    cm.CameraPointsToUVs(vtemppts,vuv);

    cm.UVToCameraPoint(vuv,vtemppts);

    cout<<"the expect result is "<<expect_point.transpose()<<endl;
    cout<<"the result from camera model is "<<vtemppts.front().transpose()<<endl;
}

void H_matrix()
{

    Eigen::Matrix3d h;

    std::vector<cv::Point2f> uvs_cv_world{cv::Point2f(0.0,0.0),cv::Point2f(1.0,0.0),
                                          cv::Point2f(1.0,1.0),cv::Point2f(0.0,1.0)};
    std::vector<cv::Point2f> uvs_cv_camera{cv::Point2f(0.0,0.0),cv::Point2f(1.0,0.0),
                                           cv::Point2f(2.0,3.0),cv::Point2f(0.0,2.0)};

    // 共线的情况无法忍受
    // std::vector<cv::Point2f> uvs_cv_world{cv::Point2f(0.0,0.0),cv::Point2f(1.0,0.0),
    //                                       cv::Point2f(2.0,0.0),cv::Point2f(3.0,0.0)};
    // std::vector<cv::Point2f> uvs_cv_camera{cv::Point2f(0.0,0.0),cv::Point2f(1.0,0.0),
    //                                        cv::Point2f(2.0,3.0),cv::Point2f(0.0,2.0)};


    auto H_cv = cv::findHomography(uvs_cv_world,uvs_cv_camera);
    cout<<"the H from opencv is "<<endl<<H_cv<<endl;

    auto temp = Converter::toVector2ds(uvs_cv_camera);

    FrameInterface::EpipolarH4Pts(Converter::toVector2ds(uvs_cv_world),
                                  Converter::toVector2ds(uvs_cv_camera),h);
    
    cout<<"the H from the frame interface is "<< endl<<h<<endl;
}

void F_matrix()
{
    // 配置八个点
    std::vector<Eigen::Vector3d> vpts_camera{
        Eigen::Vector3d(1.0,2.0,1.0),Eigen::Vector3d(2.0,1.0,3.0),
        Eigen::Vector3d(3.0,5.0,7.0),Eigen::Vector3d(3.0,8.0,2.0),
        Eigen::Vector3d(2.0,-1.0,3.0),Eigen::Vector3d(6.0,3.0,5.5),
        Eigen::Vector3d(8.0,-4.0,4.0),Eigen::Vector3d(1.5,4.0,6.0)
    };
    std::vector<Eigen::Vector3d> vtps_world;
    
    const string config ="../config/EuRoC.yaml";
    CameraModelPinhole cm(config);
    Eigen::AngleAxisd aa (0.2,Eigen::Vector3d(0.0,0.0,1.0));
    Eigen::Vector3d translation(2.0,0.2,-0.4);
    Eigen::Matrix4d transform;
    transform.setZero();
    transform.block<3,3>(0,0) = aa.matrix();
    transform.block<3,1>(0,3) = translation;
    Eigen::Matrix3d E_expect = Sophus::SO3d::hat(translation)* aa.matrix();
    Eigen::Matrix3d F_expect = cm.camera_params_.GetInstrincMatrix().transpose()*
                               E_expect*cm.camera_params_.GetInstrincMatrix();
     
    TransformPoints(vpts_camera,transform,vtps_world);
    std::vector<Eigen::Vector2d> uvs_camera,uvs_world;

    cm.CameraPointsToUVs(vpts_camera,uvs_camera);
    cm.CameraPointsToUVs(vtps_world, uvs_world);
    
    auto uvs_cv_camera = Converter::toPoints(uvs_camera);
    auto uvs_cv_world = Converter::toPoints(uvs_world);

    auto F_matrix = cv::findFundamentalMat(uvs_cv_camera,uvs_cv_world);
    cout<<"the F from opencv is "<<endl<<F_matrix<<endl;



    // Eigen::Matrix3d F_cv = Converter::toEigenM33d(F_matrix);
    // for(int i=0;i<uvs_camera.size();i++)
    // {
    //     Eigen::Vector3d uvs_camera_3d = (Eigen::Vector3d()<<uvs_camera[i],1.0).finished();
    //     Eigen::Vector3d uvs_world_3d = (Eigen::Vector3d()<<uvs_world[i],1.0).finished();
        
    //     Eigen::Vector3d temp = F_cv*uvs_camera_3d;
    //     cout<<"the reprojection error is "<< (uvs_world_3d - temp/temp.z()).transpose()<<endl;
    // }

    // Eigen::Matrix3d F;
    // FrameInterface::EpipolarF8Pts(uvs_camera,uvs_world,F);

    // cout<<"the F from FrameInterface is "<< F<<endl;

    // for(int i=0;i<uvs_camera.size();i++)
    // {
    //     Eigen::Vector3d uvs_camera_3d = (Eigen::Vector3d()<<uvs_camera[i],1.0).finished();
    //     Eigen::Vector3d uvs_world_3d = (Eigen::Vector3d()<<uvs_world[i],1.0).finished();
        
    //     Eigen::Vector3d temp = F*uvs_camera_3d;
    //     cout<<"the reprojection error is "<< (uvs_world_3d - temp/temp.z()).transpose()<<endl;
    // }
}

void EigenToMat()
{
    Eigen::Matrix<double,6,5> temp_eigen;
    temp_eigen.setZero();
    temp_eigen.block<6,1>(0,4) = (Eigen::Matrix<double,6,1>()<<1,2,3,4,5,6).finished();
    cout<<"the matrix eigen is "<<endl<<temp_eigen<<endl;
    
    auto temp_mat = Converter::toMat(temp_eigen);
    cout<<"the matrix cv is "<<endl<<temp_mat<<endl;

    std::vector<Eigen::Matrix<double,6,5>,Eigen::aligned_allocator<Eigen::Matrix<double,6,5>>> veigen;
    veigen.push_back(temp_eigen);
    veigen.push_back(temp_eigen*5);
    
    auto vtemp = Converter::toMats(veigen);
    
    for(auto & temp:vtemp)
    {
        cout<<"==========="<<endl;
        cout<<temp<<endl;
    }
}

int main()
{
F_matrix();
// H_matrix();
}

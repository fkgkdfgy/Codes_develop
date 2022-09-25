/*
 * @Author: Liu Weilong
 * @Date: 2020-10-13 10:42:12
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2020-11-29 22:08:13
 * @Description: 用于测试 和 实验icp 的匹配
 *               主要注意：1. PCL 库的API调用
 *                          |__ PCL ICP 之前总是匹配不准的问题
 *                              |__ setMaxCorrespondenceDistance 设置的数值太小 导致搜索不到
 *                                  从 0.05 调节到 0.4 之后 匹配基本上准确的
 *                              |__ transform 的方向问题 
 *                                  因为目前result_表示 一个从transformed_point_cloud 往point_cloud_的变换
 *                                  而我们一开始设置的transform 是从 pc_ 到transformed_pc_ 的变换
 *                              |__ 与VoxelFilter 一起使用
 *                                  |__ 如果 LeafSize 过大 设置 setMaxCorrespondenceDistance 会变成一个比较讨巧的问题
 *                                      因为过大的LeafSize 会让合适的匹配被判断成无匹配 然后被放弃，
 *                                      所以最好不要把 setMaxCorrespondenceDistance 设置的过小
 *                                  |__ 但是在后来的使用中发现，合理设置setMaxCorrespondenceDistance 可以一定程度上的
 *                                      转弯时造成的误匹配
 *                                所以 setMaxCorrespondenceDistance 一定要合理设置！！！
 *                          |__ NDT 匹配慢+ 匹配不准的问题 
 *                              |__ 匹配不好：setResolution 设置的过小 第一次设置 是0.05
 *                              |__ 匹配慢：没有进行下采样
 *                                         没有给初值？ 确实这样的ndt 有点慢的过分了
 *                              |__ 并且在 点和点之间有很好的一对一对应关系的时候 NDT 在相同迭代次数的情况下
 *                                        精度没有 ICP 高 
 *                          |__ GICP 特别慢
 *                       2. Eigen 的赋值运算
 *                       
 */

#include <string>
#include "pcl/io/io.h"
#include "pcl/io/pcd_io.h"
#include "pcl/common/transforms.h"
#include "pcl/registration/icp.h"
#include "pcl/registration/gicp.h"
#include "pcl/registration/ndt.h"

#include "Eigen/Eigen"
#include "filter/filter.hpp"
// #include "display/display.hpp"
using namespace std;

int main()
{
    // ============================= 数值准备 =====================================================================
    pcl::PointCloud<pcl::PointXYZ>::Ptr point_cloud_(new pcl::PointCloud<pcl::PointXYZ>());
    pcl::PointCloud<pcl::PointXYZ>::Ptr transformed_point_cloud_(new pcl::PointCloud<pcl::PointXYZ>());
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr point_cloud_with_color_(new pcl::PointCloud<pcl::PointXYZRGB>());
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr transformed_point_cloud_with_color_(new pcl::PointCloud<pcl::PointXYZRGB>());
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr all_point_cloud_with_color_(new pcl::PointCloud<pcl::PointXYZRGB>());
    string pcd_name_ = "/home/lwl/workspace/3rd-test-learning/16. pcl/registration/key_frame_0.pcd";
    pcl::io::loadPCDFile(pcd_name_.c_str(),*point_cloud_);

    // 准备变换
    Eigen::AngleAxisf angle_axis(0.05,Eigen::Vector3f(0,0,1));
    Eigen::Vector3f translation_(0.1,0,0);
    Eigen::Matrix4f transform_;
    transform_<<angle_axis.toRotationMatrix(),
                translation_,0,0,0,1;
    
    cout<<" the transform matrix is "<< endl<< 
    transform_<<endl;
    
    // 变换点云
    pcl::transformPointCloud(*point_cloud_,*transformed_point_cloud_,transform_);

    // AssignColorToPointCloud(point_cloud_,point_cloud_with_color_,255,255,255);
    // AssignColorToPointCloud(transformed_point_cloud_,transformed_point_cloud_with_color_,0,255,0);
    
    *all_point_cloud_with_color_ = *point_cloud_with_color_ + *transformed_point_cloud_with_color_;
  
    // SimpleShow(all_point_cloud_with_color_);

    cout<<"the size of point cloud is "<<point_cloud_->size()<<endl;

    // ======================================= VoxelFilter 实验 ============================================

    VoxelFilter(point_cloud_,point_cloud_,0.4);
    VoxelFilter(transformed_point_cloud_,transformed_point_cloud_,0.4);

    cout<<"the size of point cloud is "<<point_cloud_->size()<<endl;
   
    // ======================================== ICP实验 ======================================================
    
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_source_registered_(new pcl::PointCloud<pcl::PointXYZ>());
    pcl::IterativeClosestPoint<pcl::PointXYZ,pcl::PointXYZ> icp_;
    // icp_.setMaxCorrespondenceDistance(0.4);
    // icp_.setRANSACIterations(15);
    icp_.setMaximumIterations(20);
    icp_.setInputTarget(point_cloud_);
    icp_.setInputSource(transformed_point_cloud_);
    icp_.align (*cloud_source_registered_);
    
    auto result_ = icp_.getFinalTransformation ();

    cout<< " the ICP result transform is "<<endl<<
    result_<<endl;

    // 从Matix4f 转到轴角表示
    
    Eigen::AngleAxisf angle_axis_from_result_;
    Eigen::Matrix3f rotation_matrix_(result_.block(0,0,3,3));
    angle_axis_from_result_.fromRotationMatrix(rotation_matrix_);
    cout<<" the angle axis from the result is "<< angle_axis_from_result_.angle()<<endl;

    // ======================================== GICP 实验 ========================================================

    // pcl::GeneralizedIterativeClosestPoint<pcl::PointXYZ,pcl::PointXYZ> gicp_;

    // gicp_.setMaximumIterations(20);
    // gicp_.setInputTarget(point_cloud_);
    // gicp_.setInputSource(transformed_point_cloud_);
    // gicp_.align (*cloud_source_registered_);
    // result_ = gicp_.getFinalTransformation ();

    // cout<< " the GICP result transform is "<<endl<<
    // result_<<endl;

    // ======================================== NDT 实验 =====================================================

    // pcl::NormalDistributionsTransform<pcl::PointXYZ,pcl::PointXYZ> ndt_;
    // ndt_.setResolution(0.8);
    // ndt_.setMaximumIterations(20);
    // ndt_.setStepSize(0.1);
    // ndt_.setTransformationEpsilon(0.01);
    // ndt_.setInputTarget(point_cloud_);
    // ndt_.setInputSource(transformed_point_cloud_);
    // ndt_.align(*cloud_source_registered_);
    // result_ = ndt_.getFinalTransformation();
    // cout<< " the NDT result transform is "<<endl<<
    // result_<<endl;

    return 0;
}


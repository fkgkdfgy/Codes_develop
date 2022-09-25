/*
 * @Author: Liu Weilong
 * @Date: 2020-11-03 07:19:03
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2020-12-15 06:01:52
 * @Description:  基于Eigen 实现的ICP 算法 据说是又快又好  目前还是有问题  告一段落
 *                
 *                BUG 在for 的内容
 *                并且没有对transform 进行迭代
 * 
 */

#include <vector>
#include <iostream>
#include <random>

#include <sophus/se3.hpp>
#include <sophus/so3.hpp>
#include <Eigen/Eigen>
#include <g2o/stuff/sampler.h>
#include <pcl/kdtree/kdtree_flann.h>
#include <pcl/common/transforms.h>

#include "eigen_svd_icp.h"
using namespace std;

std::vector<Eigen::Vector4d> point_cloud;

void buildPointCloud()
{
    for(int i = 0;i<30;i++)
    {
        Eigen::Vector4d tmp_point;
        tmp_point<< i * 0.3,i*0.3,i*0.3,1;
        point_cloud.push_back(tmp_point);
    }

    for(int i = 0;i<30;i++)
    {
        Eigen::Vector4d tmp_point;
        tmp_point<< 0.5,i*0.3 + i* 0.02,i*0.3,1;
        point_cloud.push_back(tmp_point);
    }
    
    for(int i = 0;i<30;i++)
    {
        Eigen::Vector4d tmp_point;
        tmp_point<< 0.9,i*0.3 + i* 0.04,i*0.3 - i*0.01,1;
        point_cloud.push_back(tmp_point);
    }    
}

bool transformPointCloud(const Sophus::SE3d & pose_camera_to_world, 
                         const vector<Eigen::Vector4d> & points_in_world,
                         vector<Eigen::Vector4d> & points_in_camera)
{
    if(points_in_world.size() == 0)
    {
        std::cout<<"[Bad Data]: the size of point array is 0.Thus return false!"<<std::endl;
        return false;
    }
    
    Eigen::Matrix4d pose_world_to_camera = pose_camera_to_world.matrix().inverse();
    points_in_camera.clear();
    points_in_camera.reserve(points_in_world.size());

    for(const auto & point:points_in_world)
    {
        Eigen::Vector4d tmp_point = pose_world_to_camera * point;
        points_in_camera.push_back(tmp_point);
    }
}

// 这里使用高斯分布的误差(0,1) 高斯分布
// TODO 这里的参数可以进行调节，如果出问题可以注意这里
bool AddNoise(std::vector<Eigen::Vector4d> & point_cloud)
{
    // scale 再大基本就挂了  因为点在设置的时候就比较密集
    double scale = 0.001;
    Eigen::Matrix3d covar = Eigen::Matrix3d::Identity()*scale;
    g2o::GaussianSampler<Eigen::Matrix<double,3,1>,Eigen::Matrix<double,3,3>> sampler;
    sampler.setDistribution(covar);
    for(auto & point :point_cloud)
    {
        Eigen::Vector4d noise;
        noise<<sampler.generateSample(),0;
        // cout<<"the noise is "<<noise.transpose()<<endl;
        point = point + noise;
    }
}

template <typename T>
void ToPclPointType(const std::vector<Eigen::Vector4d> & point_array, 
                    pcl::PointCloud<T> & point_cloud)
{
    // CHECK_NE(point_array.size(),0);
    point_cloud.clear();
    for(auto & point:point_array)
    {
        T tmp_point(point(0),point(1),point(2));
        point_cloud.push_back(tmp_point);
    }
}

template <typename T>
class EigenSVDICP
{
    public:

    EigenSVDICP(const std::string & config_path);
    


    private:


    private:
    
    pcl::KdTree<T> kdtree_;
    class config
    {
        public:
        int max_iter_;
        double euclidean_eps_;
        double euclidean_change_;
        double max_dist_;
    }
};


int main()
{
    buildPointCloud();

    // ================================ 生成变化 ================================
    double transform_lie [6] {0.2,0.5,0.6,0.0,0.0,0.0};
    Eigen::Map<Eigen::Matrix<double,6,1>> lie_map(transform_lie);
    Sophus::SE3d transform_se3 = Sophus::SE3d::exp(lie_map);
    
    cout<<"the targeted transform is "<< endl
        << transform_se3.matrix()<<endl;

    // ================================ 点云变换 ================================

    std::vector<Eigen::Vector4d> point_in_camera;
    transformPointCloud(transform_se3,point_cloud,point_in_camera);
    // AddNoise(point_in_camera);

    // ================================ 变换到pcl类型并存入Kdtree ================

    pcl::PointCloud<pcl::PointXYZ>::Ptr point_cloud_in_world (new pcl::PointCloud<pcl::PointXYZ>());
    pcl::PointCloud<pcl::PointXYZ>::Ptr point_cloud_in_camera(new pcl::PointCloud<pcl::PointXYZ>());
    ToPclPointType(point_cloud,*point_cloud_in_world);
    ToPclPointType(point_in_camera,*point_cloud_in_camera);

    pcl::KdTreeFLANN<pcl::PointXYZ> kdtree;

    // ================================ 准备params =============================

    int max_iter = 30;
    double euclidean_change = 1e-3;
    double euclidean_eps = 1e-2;
    double max_dis = 3.0;
    Eigen::Matrix3f R = Eigen::Matrix3f::Identity();
    Eigen::Vector3f t = Eigen::Vector3f::Zero();
    
    double squared_distance_th = max_dis * max_dis;
    double cur_squared_dist = 0.0;
    double last_squared_dist = std::numeric_limits<double>::max();


    Eigen::Vector3f target_center;
    Eigen::Vector3f source_center;
    std::vector<int> search_index;
    std::vector<float> search_dis;
    std::vector<Eigen::Vector3f> target_match;
    std::vector<Eigen::Vector3f> source_match;
    kdtree.setInputCloud(point_cloud_in_world);
    Eigen::Matrix4f result_transform = Eigen::Matrix4f::Identity();
    pcl::PointCloud<pcl::PointXYZ>::Ptr tmp_point_cloud(new pcl::PointCloud<pcl::PointXYZ>());
    pcl::PointCloud<pcl::PointXYZ>::Ptr predict_transformed_point_cloud(new pcl::PointCloud<pcl::PointXYZ>());

    double predict_lie [6] {0.1,0.4,0.3,0.0,0.0,0.0};
    Eigen::Map<Sophus::SE3d> predict_SE3(predict_lie);
    pcl::transformPointCloud(*point_cloud_in_camera,*predict_transformed_point_cloud,predict_SE3.matrix().cast<float>());
    for(int i=0;i<max_iter;i++)
    {
        


        double sum_squared_dist = 0.0;

        target_match.clear();
        source_match.clear();
        target_center = Eigen::Vector3f::Zero();
        source_center = Eigen::Vector3f::Zero();
        
        Eigen::Matrix4f transform = Eigen::Matrix4f::Identity();
        transform.block(0,0,3,3)=R;
        transform.block(0,3,3,1)=t;
        result_transform = transform*result_transform;

        pcl::transformPointCloud(*predict_transformed_point_cloud,*tmp_point_cloud,result_transform);

        // get Correspondence
        for(auto & point:tmp_point_cloud->points)
        {
            if(!kdtree.nearestKSearch(point,1,search_index,search_dis))
            {
                std::cerr<<"there is something wrong with the kdtree!"<<std::endl;
            }
            if(search_dis[0] < squared_distance_th)
            {
                sum_squared_dist += search_dis[0];
                auto & points = point_cloud_in_world->points;
                int index = search_index[0];
                target_match.emplace_back(points[index].x,points[index].y,points[index].z);
                source_match.emplace_back(point.x,point.y,point.z);
            }
        }

        // 构建SVD
        for(int i = 0;i<target_match.size();i++)
        {
            target_center += target_match[i];
            source_center += source_match[i];
        }

        if(target_match.size()<5)
        {
            std::cerr<<"no match exist!"<<std::endl;
            abort();
        }

        target_center = target_center/float(target_match.size());
        source_center = source_center/float(source_match.size());        
        
        // 这里是一个不同点
        Eigen::Matrix3f W = Eigen::Matrix3f::Zero();
        for(int i = 0;i<source_match.size();i++)
        {
            W += (target_match.at(i) -target_center) *(source_match.at(i)-source_center).transpose();
        }
                    
        Eigen::JacobiSVD<Eigen::Matrix3f> svd(W,Eigen::ComputeFullU|Eigen::ComputeFullV);
        R = svd.matrixV() * svd.matrixU().transpose();
        t = target_center - R*source_center;

        cur_squared_dist = sum_squared_dist/float(source_match.size());
        double squared_dist_change = last_squared_dist - cur_squared_dist;
        last_squared_dist = cur_squared_dist;
        if(squared_dist_change<euclidean_change*euclidean_change || cur_squared_dist < euclidean_eps * euclidean_eps)
        {
            break;
        }
        
    }

        Eigen::Matrix4f transform = Eigen::Matrix4f::Identity();
        transform.block(0,0,3,3)=R;
        transform.block(0,3,3,1)=t;
        result_transform = transform*result_transform*predict_SE3.matrix().cast<float>();
        std::cout<<"the esitimation is "<<std::endl
                 <<result_transform<<std::endl;

}
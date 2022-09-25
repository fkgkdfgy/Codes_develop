/*
 * @Author: Liu Weilong
 * @Date: 2020-12-28 21:25:13
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2020-12-28 22:17:58
 * @Description: 
 */
#include "g2o_icp_so3_r3.h"

#include <vector>
#include <iostream>
#include <random>

#include <ceres/ceres.h>
#include <sophus/se3.hpp>
#include <sophus/so3.hpp>
#include <Eigen/Eigen>
#include <g2o/stuff/sampler.h>

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

int main()
{
    buildPointCloud();
    const double transform_lie [6] {0.4,0.6,0.7,0.6,0.7,0.8};
    Eigen::Map<const Eigen::Matrix<double,6,1>> transform_map(transform_lie);
    Sophus::SE3<double> transform_se3 = Sophus::SE3d::exp(transform_map);
    Sophus::SE3<double> Tcw = transform_se3;
    std::vector<Eigen::Vector4d> point_in_camera;
    transformPointCloud(Tcw,point_cloud,point_in_camera);
    AddNoise(point_in_camera);

    typedef g2o::BlockSolver<g2o::BlockSolverTraits<6,3>> BlockSolverType;
    typedef g2o::LinearSolverDense<BlockSolverType::PoseMatrixType> LinearSolverType;
    
    // 创建求解器
    auto solver = new g2o::OptimizationAlgorithmLevenberg(
        g2o::make_unique<BlockSolverType>(g2o::make_unique<LinearSolverType>()));
    g2o::SparseOptimizer optimizer;     // 图模型
    optimizer.setAlgorithm(solver);   // 设置求解器
    optimizer.setVerbose(true);       // 打开调试输出
    
    auto & point_in_world =  point_cloud;

    double initial_lie [6] {0.44,0.65,0.72,0.55,0.73,0.86};
    Eigen::Vector3d _so3(initial_lie+3);
    Eigen::Vector3d _r3(initial_lie);
    SO3R3 Twc;
    Twc._r3 = _r3;
    Twc._SO3 = Sophus::SO3d::exp(_so3);
    SO3R3Vertex * vx = new SO3R3Vertex();
    vx->setEstimate(Twc);
    vx->setId(0);
    
    optimizer.addVertex(vx);
    for(int i =0; i<point_in_camera.size();i++)
    {
        EdgeICP * e = new EdgeICP();
        e->Xw = point_in_camera[i].block<3,1>(0,0);
        e->setVertex(0,vx);
        e->setMeasurement(point_in_world[i].block<3,1>(0,0));
        e->setInformation(Eigen::Matrix3d::Identity());
        optimizer.addEdge(e);
    }

    cout << "start optimization" << endl;
    chrono::steady_clock::time_point t1 = chrono::steady_clock::now();
    optimizer.initializeOptimization();
    optimizer.optimize(10);
    chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
    chrono::duration<double> time_used = chrono::duration_cast<chrono::duration<double>>(t2 - t1);
    cout << "solve time cost = " << time_used.count() << " seconds. " << endl;
    cout<<"the real is "<<endl<<transform_se3.matrix()<<endl;
    cout<<"the so3 is "<<endl<<vx->estimate()._SO3.matrix()<<endl;
    cout<<"the r3  is "<<endl<<vx->estimate()._r3<<endl;
    

    return 0;
}
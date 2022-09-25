/*
 * @Author: Liu Weilong
 * @Date: 2021-01-18 13:26:07
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-01-18 15:40:37
 * @FilePath: /3rd-test-learning/22. slam_demo/ground_constraint/test_pool.cpp
 * @Description: 
 *              
 *              主要包括的测试内容：
 *              1. RPCheck      用于测试 XY 角度上的变化 查看是不是可以进行算法优化
 *              2. LoadParams   检查读取
 * 
 */
#include <iostream>
#include <cmath>

#include "Eigen/Eigen"
#include "sophus/so3.hpp"

#include "ground_constraint_builder.h"

#include <g2o/core/block_solver.h>
#include <g2o/core/optimization_algorithm_levenberg.h>
#include <g2o/core/optimization_algorithm_gauss_newton.h>
#include <g2o/core/optimization_algorithm_dogleg.h>
#include <g2o/solvers/dense/linear_solver_dense.h>

using namespace std;

void RPCheck()
{
    Eigen::AngleAxisd x_delta(0.01,Eigen::Vector3d(1.0,0.0,0.0));
    Sophus::SO3d x_so3_delta= Sophus::SO3d::exp(x_delta.angle()*x_delta.axis());
    Eigen::AngleAxisd y_delta(0.01,Eigen::Vector3d(0.0,1.0,0.0));
    Sophus::SO3d y_so3_delta= Sophus::SO3d::exp(y_delta.angle()*y_delta.axis());
    for(double angle =0.0;angle<2*M_PI;angle+=0.01)
    {
        Eigen::AngleAxisd z(angle,Eigen::Vector3d(0.0,0.0,1.0));
        Sophus::SO3d Z_so3 = Sophus::SO3d::exp(z.angle()*z.axis());
        Sophus::SO3d whole_SO3 = Z_so3*x_so3_delta*y_so3_delta;
        cout<<"the delta xy in whole SO3 is "<<whole_SO3.log().head<2>().transpose()<<endl;
    }
}

void LoadParams()
{
    auto temp = EdgeRPZSO3R3::GetExtParams();
}

void OptPose()
{
    typedef g2o::LinearSolverDense<g2o::BlockSolverX::PoseMatrixType> LinearSolverType;
    
    // 创建求解器
    auto solver = new g2o::OptimizationAlgorithmDogleg(
        g2o::make_unique<g2o::BlockSolverX>(g2o::make_unique<LinearSolverType>()));
    g2o::SparseOptimizer optimizer;     // 图模型
    optimizer.setAlgorithm(solver);   // 设置求解器
    optimizer.setVerbose(true);       // 打开调试输出

    auto T_camera_in_baselink = EdgeRPZSO3R3::GetExtParams();
    



}
int main()
{
   LoadParams();
}
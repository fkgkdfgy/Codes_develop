/*
 * @Author: Liu Weilong
 * @Date: 2020-10-26 07:16:54
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2020-11-29 21:19:56
 * @Description: 主要是为了测试和学习LocalParameter 
 *              
 *               ie.   Analystic Diff SE3 上的优化
 *                     在一个机缘巧可下  见证了SE3 对比于Quternion + R3 惊人的稳定性
 *                     两版ceres_icp 都是 lie[6] 忘记exp 变换
 *                     就直接赋给了 Sophus::SE3d show_lie   这个时候的 show_lie 已经严重不是SE3了
 *                     结果在优化的时候SE3 直接给掰成了正确答案
 *                     Quternion + R3 虽然也有优化效果，但是和正确答案相差较远
 * 
 *                     这里提供的是一个T*p 的SE3 优化模型
 * 
 *                     
 *                     去掉了测试的部分
 *                     另外包含从Matrix4f 创建SE3
 *                     
 */

#include <vector>
#include <iostream>
#include <random>

#include <ceres/ceres.h>
#include <sophus/se3.hpp>
#include <sophus/so3.hpp>
#include <Eigen/Eigen>
#include <g2o/stuff/sampler.h>
#include <pcl/kdtree/kdtree_flann.h>
#include <pcl/common/transforms.h>

#include "common.h"
using namespace std;

std::vector<Eigen::Vector4d> point_cloud;

class AnalyticCostFunction:public ceres::SizedCostFunction<3,6>
{
    public:
    AnalyticCostFunction(const Eigen::Vector3d watched_point, const Eigen::Vector3d point_in_world):
                         watched_point_(watched_point),point_in_world_(point_in_world){}

    virtual bool Evaluate(double const * const * params,
                          double * residuals,
                          double **jacobians)const
    {
        Eigen::Map<Eigen::Matrix<double,3,1>> residual_map(residuals);
        Eigen::Map<const Eigen::Matrix<double,6,1>> lie_map(params[0]);
        // cout<<" the params is "<<lie_map.transpose()<<endl;
        Sophus::SE3<double> se3 = Sophus::SE3<double>::exp(lie_map);
        
        residual_map = se3*watched_point_ -point_in_world_;

        if (!jacobians) return true;
        double* jacobian = jacobians[0];
        if (!jacobian) return true;
        
        // 这里不使用Sophus 的SE3 求导
        // 因为Sophus的SE3 是四元数 + 位姿的一个组合

        Eigen::Map<Eigen::Matrix<double,3,6>> jacobian_map(jacobians[0]);
        
        Eigen::Matrix<double,4,6> point_jing = Eigen::Matrix<double,4,6>::Zero();

        // auto t_p = se3 * watched_point_;

        point_jing.block(0,0,3,3) = Eigen::Matrix<double,3,3>::Identity();

        point_jing.block(0,3,3,3) = -1*Sophus::SO3<double>::hat(watched_point_);
        
        auto result_4_6 = se3.matrix()*point_jing;
        
        auto jac = result_4_6.block(0,0,3,6);

        int count =0;
        for(int i = 0;i<3;i++)
        {
            for(int j =0;j<6;j++)
            {
                jacobian[count] = result_4_6(i,j);
                count++;
            }
        }
        return true;    
    }
    
    private:
    Eigen::Vector3d watched_point_;
    Eigen::Vector3d point_in_world_;
};

class SE3LocalParameterizaton:public ceres::LocalParameterization
{
    virtual bool Plus(const double* x,
                const double* delta,
                double* x_plus_delta) const 
    {
        
        Eigen::Map<const Eigen::Matrix<double,6,1>> lie(x);
        Eigen::Map<const Eigen::Matrix<double,6,1>> delta_lie(delta);
        // cout<< "show the delta"<< delta_lie.transpose()<<endl;
        Eigen::Map<Eigen::Matrix<double,6,1>> new_lie(x_plus_delta);
        Sophus::SE3<double> so3_rotation = Sophus::SE3<double>::exp(lie);
        Sophus::SE3<double> so3_delta_rotation = Sophus::SE3<double>::exp(delta_lie);
        
        auto result = so3_rotation*so3_delta_rotation;
        
        new_lie = result.log();

        return true;
    }

    virtual bool ComputeJacobian(const double* x, double* jacobian) const
    {
        ceres::MatrixRef(jacobian, 6, 6) = ceres::Matrix::Identity(6, 6);
        return true;
    }

    virtual int GlobalSize() const { return 6; }
    virtual int LocalSize() const { return 6; }
};




int main()
{
    buildPointCloud();
    Eigen::Matrix4d transform = Eigen::Matrix4d::Identity();
    const double initial_lie [6] {0.4,0.6,0.7,0.6,0.7,0.8};
    Eigen::Map<const Eigen::Matrix<double,6,1>> k_lie(initial_lie);
    Sophus::SE3<double> se3 = Sophus::SE3d::exp(k_lie);
    
    std::vector<Eigen::Vector4d> point_in_camera;
    transformPointCloud(se3,point_cloud,point_in_camera);
    AddNoise(point_in_camera);

    cout<<" show the transform matrix from SE3"<<endl
        <<se3.matrix()<<endl;
    
    assert(point_in_camera.front()(3) == 1);
    assert(point_in_camera.back()(3) == 1);    

    ceres::Problem problem;
    double lie [6] {0.44,0.65,0.72,0.55,0.73,0.86};

    // 这个地方明明是错的  LieGroup 惊人的稳定性 还是给掰过来了
    Eigen::Map<Sophus::SE3d> show_lie(lie);
    
    cout<<" the right SE3 is "<<endl
        << show_lie.matrix()<<endl;
    // 现在这么看的的确是节省了一些添加 LocalParameterization 的代码
    problem.AddParameterBlock(lie,6,new SE3LocalParameterizaton());
    for(int i = 0; i<point_in_camera.size();i++)
    {
        ceres::CostFunction * cf = new AnalyticCostFunction(point_in_camera[i].segment(0,3),point_cloud[i].segment(0,3));
        problem.AddResidualBlock(cf,new ceres::CauchyLoss(0.5),lie);       
    }
   

    ceres::Solver::Options options;
    ceres::Solver::Summary summary;
    options.linear_solver_type = ceres::DENSE_QR;
    options.minimizer_type = ceres::TRUST_REGION;
    options.trust_region_strategy_type = ceres::DOGLEG;
    options.minimizer_progress_to_stdout = true;

    // 这个Solve 和那个不能用的solve 看起来还是有区分度的
    ceres::Solve(options,&problem,&summary);
    Eigen::Map<Eigen::Matrix<double,6,1>> lie_map(lie);
    cout<<summary.BriefReport()<<endl;
    cout<<"the initial lie is "<< k_lie.transpose()<<endl
        <<"the result lie is "<< lie_map.transpose()<<endl;
    
    

};

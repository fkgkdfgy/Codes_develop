/*
 * @Author: Liu Weilong
 * @Date: 2020-10-17 21:41:02
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2020-10-30 08:07:48
 * @Description:  基于ceres 的ndt 算法
 *                现在换成Sophus ::SE3 的方法效果也还是不行
 *                目前看起来这个部分的确是正确的
 */
#include <iostream>
#include <vector>
#include "Eigen/Eigen"
#include "sophus/so3.hpp"
#include "sophus/se3.hpp"
#include "glog/logging.h"
#include "ceres/ceres.h"
#include "pcl/filters/voxel_grid_covariance.h"

class NDTLoss: public ceres::LossFunction
{
    public:
    explicit NDTLoss(double resolution,double outlier_ratio)
    {
        double gauss_c1, gauss_c2;
        double outlier_ratio_ = outlier_ratio;
        double resolution_ = resolution;
        // Initializes the guassian fitting parameters (eq. 6.8) [Magnusson 2009]
        gauss_c1 = 10.0 * (1 - outlier_ratio_);
        gauss_c2 = outlier_ratio_ / pow (resolution_, 3);
        gauss_d3_ = -log (gauss_c2);
        gauss_d1_ = -log ( gauss_c1 + gauss_c2 ) - gauss_d3_;
        gauss_d2_ = -2 * log ((-log ( gauss_c1 * exp ( -0.5 ) + gauss_c2 ) - gauss_d3_) / gauss_d1_);
    }

    virtual void Evaluate(double s,double rho[3] )const
    {
        rho[0] = -gauss_d1_ * std::exp(-1*gauss_d2_*s);
        rho[1] = (-1*gauss_d2_)*rho[0];
        rho[2] = (-1*gauss_d2_)*rho[1];
    }

    private:
    double gauss_d1_,gauss_d2_,gauss_d3_;
};


class NDTCostFunctorAnalystic: public ceres::SizedCostFunction<3,6>
{
    public:

    NDTCostFunctorAnalystic(const Eigen::Matrix3d & covar, 
                            const Eigen::Vector3d & mean,
                            const Eigen::Vector3d & point):
                            mean_(mean),point_(point)
    {
        Eigen::LLT<Eigen::Matrix3d> llt_of_info(covar.inverse());
        // TODO 一开始这里没有进行转置 ， 感觉此处应该有一个转置，所以目前先改为转置版本 如果有问题再改回来
        // TODO 目前感觉转不转效果不是很明显
        lt_matrix_ = llt_of_info.matrixL().toDenseMatrix();
        // LOG(INFO)<<"A  cost function is built"<<std::endl;
    }
    
    //  这里使用Sophus 的 SE3 四元数和位移进行表示
    virtual
    bool Evaluate(const double* const * params, double* resiudal,double ** jacobians)const
    {
        if(params ==NULL)
        return true;
        Eigen::Map<const Eigen::Matrix<double,6,1>> lie_map(params[0]);
        Eigen::Map<Eigen::Vector3d> residual_map(resiudal);
        Sophus::SE3<double> se3 = Sophus::SE3d::exp(lie_map);
        
        residual_map = se3*point_ - mean_;
        residual_map = lt_matrix_ * residual_map;
        if(!jacobians) return true;
        double * jacobian = jacobians[0];
        if(!jacobian) return true;

        Eigen::Matrix<double,3,6> point_jing;
        point_jing.block(0,0,3,3) = Eigen::Matrix<double,3,3>::Identity();

        point_jing.block(0,3,3,3) = -1*Sophus::SO3<double>::hat(point_);
        
        point_jing = lt_matrix_*point_jing;
        
        int count =0;
        for(int i = 0;i<3;i++)
        {
            for (int j = 0;j<6;j++)
            {
                jacobian[count] = point_jing(i,j);
                count++;
            }
        }


        return true;
    }
    
    private:

    Eigen::Vector3d mean_;
    Eigen::Vector3d point_;
    Eigen::Matrix3d lt_matrix_;
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


class NDTProblem
{
    public:
    
    void setInputSource(const pcl::PointCloud<pcl::PointXYZ>::ConstPtr cloud);

    void setInputTarget(const pcl::PointCloud<pcl::PointXYZ>::ConstPtr cloud);

    void setResolution(double voxel_size){
        leaf_size_ = voxel_size;
        resolution_ = voxel_size;
    }
    
    void setRadius(double radius){

    }

    void buildProlemAndSolve(Eigen::Matrix4f & result, Sophus::SE3d se3);

    private:
    pcl::VoxelGridCovariance<pcl::PointXYZ> voxel_covar_;
    pcl::PointCloud<pcl::PointXYZ>::ConstPtr source_point_cloud_;
    pcl::PointCloud<pcl::PointXYZ>::ConstPtr target_point_cloud_;
    double leaf_size_;
    double radius_;
    double resolution_;
};

void NDTProblem::setInputSource(const pcl::PointCloud<pcl::PointXYZ>::ConstPtr cloud)
{
    source_point_cloud_ = cloud;
}

void NDTProblem::setInputTarget(const pcl::PointCloud<pcl::PointXYZ>::ConstPtr cloud)
{
    target_point_cloud_ = cloud;
    voxel_covar_.setLeafSize(resolution_,resolution_,resolution_);
    voxel_covar_.setInputCloud(target_point_cloud_);
    voxel_covar_.filter(true);
}

void NDTProblem::buildProlemAndSolve(Eigen::Matrix4f & result, Sophus::SE3d se3)
{
    

    if(source_point_cloud_ == nullptr ||target_point_cloud_ == nullptr
       ||source_point_cloud_->empty()||target_point_cloud_->empty())
       {
           return;
       }
    
    ceres::Problem problem;

    Eigen::Matrix<double,6,1> lie = se3.log();

    for(auto & point:source_point_cloud_->points)
    {
        Eigen::Vector3d point_v3;
        point_v3<<point.x,point.y,point.z;
        std::vector<pcl::VoxelGridCovariance<pcl::PointXYZ>::LeafConstPtr> k_leaves;
        std::vector<float> distancess;
        voxel_covar_.radiusSearch(point,resolution_,k_leaves,distancess);
        // LOG(INFO)<<"the size of k_leaves is "<<k_leaves.size()<<std::endl;
        for (auto leaf:k_leaves)
        {
            // LOG(INFO)<<leaf->getPointCount()<<std::endl;
            ceres::CostFunction * cf = new NDTCostFunctorAnalystic(leaf->cov_,leaf->mean_,point_v3);
            // TODO LossFunction 看起来有些问题
            // problem.AddResidualBlock(cf,new NDTLoss(resolution_,0.65),lie.data());
            problem.AddResidualBlock(cf,new ceres::CauchyLoss(1),lie.data());
        }
    }

    problem.SetParameterization(lie.data(),new SE3LocalParameterizaton());

    ceres::Solver::Options options;
    ceres::Solver::Summary summary;
    options.linear_solver_type = ceres::DENSE_QR;
    options.minimizer_type = ceres::TRUST_REGION;
    options.trust_region_strategy_type = ceres::LEVENBERG_MARQUARDT;

    ceres::Solve(options,&problem,&summary);
    
    std::cout<<summary.BriefReport()<<std::endl;

    std::cout<<"the esitmation transform is "<<std::endl<<lie.transpose()<<std::endl;   
}


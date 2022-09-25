/*
 * @Author: Liu Weilong
 * @Date: 2020-10-12 21:27:47
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-02-21 22:04:01
 * @Description: 
 */
#pragma once
#include "pcl/registration/gicp.h"
#include "ceres/ceres.h"
#include "pcl/kdtree/kdtree_flann.h"
#include "pcl/io/pcd_io.h"
#include "sophus/se3.hpp"

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



namespace lidar_localization {
class ICPRegistration: public RegistrationInterface {
  public:

    ICPRegistration():kdtree_(new pcl::KdTreeFLANN<CloudData::POINT>())
    {
        LOG(INFO) << "new ICP "<<std::endl;
        max_dist_squared = 8;
    }

    bool SetInputTarget(const CloudData::CLOUD_PTR& input_target) override
    {
        
        kdtree_->setInputCloud(input_target);
        return true;
    }

    bool ICPRegistrationSingle(pcl::PointCloud<pcl::PointXYZ>::Ptr t_pc_ptr,pcl::KdTreeFLANN<pcl::PointXYZ>::Ptr kdtree_ptr,
                              Eigen::Matrix4f predict_pose, Eigen::Matrix4f & result_pose, 
                              bool & finish,
                              float max_dis_squared = 3,
                              float min_err2 = 0.015)
    {
        // 调节正交性 因为坐标转换方式的原因 注释掉了 正交性调节的环节
        // Sophus::SE3d init_se3 = Sophus::SE3d::trans(predict_pose.block(0,3,3,1));
        // Eigen::AngleAxisd angle_axis(Eigen::Matrix3d(predict_pose.block(0,0,3,3)));
        // init_se3.setRotationMatrix(angle_axis.toRotationMatrix());
        // cout<<" show the inital matrix"<<endl
        //     << init_se3.matrix()<<endl;
        // cout<<" show the initial lie "<<endl
        //     << init_se3.log().transpose()<<endl;

        Eigen::Matrix<double,6,1> delta_transform = Eigen::Matrix<double,6,1>::Zero();
        ceres::Problem problem;
        const auto & target_pc = kdtree_ptr->getInputCloud()->points;

        problem.AddParameterBlock(delta_transform.data(),6,new SE3LocalParameterizaton());
        
        // 进行匹配
        std::vector<int> search_index_array;
        std::vector<float> search_dis_array;
        std::vector<Eigen::Vector3d> watched_point_matched;
        std::vector<Eigen::Vector3d> point_in_world_matched;
        for( auto & point: t_pc_ptr->points)
        {
            if(! kdtree_ptr->nearestKSearch(point,1,search_index_array,search_dis_array))
            {
                return false;
            }
            if(search_dis_array[0]<max_dis_squared)
            {
                watched_point_matched.push_back (point.getVector3fMap().cast<double>());
                point_in_world_matched.push_back(target_pc.at(search_index_array[0]).getVector3fMap().cast<double>());
            }
        }

        double scale_factor = std::sqrt(1.0/double(watched_point_matched.size()));

        for(int i =0;i<watched_point_matched.size();i++)
        {   
            
            ceres::CostFunction * cf = new AnalyticCostFunction(watched_point_matched[i],point_in_world_matched[i]);
            problem.AddResidualBlock(cf,new ceres::HuberLoss(0.1),delta_transform.data());
        }

        ceres::Solver::Options options;
        ceres::Solver::Summary summary;
        options.linear_solver_type = ceres::DENSE_QR;
        options.minimizer_type = ceres::TRUST_REGION;
        options.trust_region_strategy_type = ceres::LEVENBERG_MARQUARDT;
        
        options.max_num_iterations = 8;
        options.function_tolerance = 1e-3;
        
        

        // 这个Solve 和那个不能用的solve 看起来还是有区分度的
        ceres::Solve(options,&problem,&summary);
        // std::cout<<summary.BriefReport()<<endl;
        
        result_pose = Sophus::SE3d::exp(delta_transform).matrix().cast<float>()*predict_pose;

        // auto temp = summary.final_cost/double(watched_point_matched.size());
        // cout<<" the final in every point is "<<temp<<endl;
        if(summary.final_cost/double(watched_point_matched.size()) < min_err2)
        {
            finish = true;
        }
        return true;
    }

    bool ScanMatch(const CloudData::CLOUD_PTR& input_source, 
                   const Eigen::Matrix4f& predict_pose, 
                   CloudData::CLOUD_PTR& result_cloud_ptr,
                   Eigen::Matrix4f& result_pose) override
    {
        pcl::PointCloud<pcl::PointXYZ>::Ptr transformed_source_point_cloud(new pcl::PointCloud<pcl::PointXYZ>());
        pcl::transformPointCloud(*input_source,*transformed_source_point_cloud,predict_pose);
        Eigen::Matrix4f iterative_result = predict_pose;
        bool finish = false;
        for(int i =0;i<10;i++)
        {
            ICPRegistrationSingle(transformed_source_point_cloud,kdtree_,iterative_result,iterative_result,finish);
            if(finish)
            {
                std::cout<<"Totally, "<<i<<" th iterations"<<std::endl;
                break;
            }
            // std::cout<<"print the iterative result matrix :"<<endl
            //     << iterative_result<<endl;
            pcl::transformPointCloud(*input_source,*transformed_source_point_cloud,iterative_result);
        }
        result_pose = iterative_result;

    }
  
  private:


  private:
    pcl::KdTreeFLANN<CloudData::POINT>::Ptr kdtree_;
    float max_dist_squared;
};
}

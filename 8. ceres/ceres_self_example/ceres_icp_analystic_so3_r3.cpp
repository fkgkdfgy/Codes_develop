/*
 * @Author: Liu Weilong
 * @Date: 2020-11-23 19:05:34
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-01-06 11:09:37
 * @Description: 主要是用于SO3 的测试
 *               1. SO3 自己的求导是否正确  是正确的
 *               2. SO3 非扰动和扰动模型的测试
 *                  |__ 扰动基本正确 循环四次结束
 *                                 去掉SO3PlusOnly 之后也需要大概17次才能够收敛
 *                  |__ 非扰动版本   循环40几次感觉是有一些问题
 * 
 * 
 *               不知道为什么扰动模型可以更快的收敛
 */


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


Eigen::Matrix3d RightJacobianSO3(const float &x, const float &y, const float &z)
{
    static float eps = 1e-4;
    Eigen::Matrix3d I = Eigen::Matrix3d::Identity();
    const float d2 = x*x+y*y+z*z;
    const float d = sqrt(d2);
    Eigen::Matrix3d W =Eigen::Matrix3d::Identity();
    W << 0, -z, y,
         z, 0, -x,
         -y,  x, 0;
        return I - W*(1.0f-cos(d))/d2 + W*W*(d-sin(d))/(d2*d);
}


// SO3 绕动模型
class SO3R3ICPError:public ceres::SizedCostFunction<3,3,3>
{
    public:
    SO3R3ICPError(const Eigen::Vector3d watched_point, const Eigen::Vector3d point_in_world):
                         watched_point_(watched_point),point_in_world_(point_in_world){}

    virtual bool Evaluate(double const * const * params,
                          double * residual,
                          double **jacobians)const
    {
        Eigen::Map<const Eigen::Vector3d> so3_map(params[0]);
        Eigen::Map<const Eigen::Vector3d> translation_map(params[1]);
        Eigen::Map<Eigen::Vector3d> residual_map(residual);
        
        residual_map = Sophus::SO3d::exp(so3_map)*watched_point_+translation_map -point_in_world_;

        if (!jacobians) return true;
        double* jacobian = jacobians[0];
        if (!jacobian) return true;
        Eigen::Matrix<double,3,3> jacobian_R = Eigen::Matrix<double,3,3>::Identity();
        jacobian_R = -1*Sophus::SO3d::exp(so3_map).matrix()*Sophus::SO3d::hat(watched_point_);
        Eigen::Matrix<double,3,3> jacobian_t = Eigen::Matrix<double,3,3>::Identity();
        
        int count = 0;
        for(int i=0;i<3;i++)
        {
            for(int j=0;j<3;j++)
            {
                jacobians[0][count] = jacobian_R(i,j);
                count++;
            }
        }
        count = 0;
        for(int i=0;i<3;i++)
        {
            for(int j=0;j<3;j++)
            {
                jacobians[1][count] = jacobian_t(i,j);
                count++;
            }
        }   
        return true;
    }

    static ceres::CostFunction * Create(const Eigen::Vector3d watched_point, 
                                        const Eigen::Vector3d point_in_world)
    {
        return new SO3R3ICPError(watched_point,point_in_world);
    }
    private:
    Eigen::Vector3d watched_point_;
    Eigen::Vector3d point_in_world_;
};

class SO3PlusOnlyLocalParameterization: public ceres::LocalParameterization
{
    public:
    
    virtual bool Plus(const double* x,
                const double* delta,
                double* x_plus_delta) const 
    {
        Eigen::Map<const Eigen::Vector3d> delta_x_map(delta);
        Eigen::Map<const Eigen::Vector3d> x_map(x);
        Eigen::Map< Eigen::Vector3d> new_so3(x_plus_delta);

        auto x_SO3 = Sophus::SO3d::exp(x_map);
        auto delta_x_SO3 = Sophus::SO3d::exp(delta_x_map);

        new_so3 = (x_SO3*delta_x_SO3).log();

        return true;
    }

    virtual bool ComputeJacobian(const double* x, double* jacobian) const
    {
        ceres::MatrixRef(jacobian, 3, 3) = ceres::Matrix::Identity(3, 3);
        return true;
    }

    virtual int GlobalSize() const { return 3; }
    virtual int LocalSize() const { return 3; }
};

// SO3 非绕动模型
class SO3R3ICPErrorNoneDelta:public ceres::SizedCostFunction<3,3,3>
{
    public:
    SO3R3ICPErrorNoneDelta(const Eigen::Vector3d watched_point, const Eigen::Vector3d point_in_world):
                           watched_point_(watched_point),point_in_world_(point_in_world){}

    virtual bool Evaluate(double const * const * params,
                          double * residual,
                          double **jacobians)const
    {
        Eigen::Map<const Eigen::Vector3d> so3_map(params[0]);
        Eigen::Map<const Eigen::Vector3d> translation_map(params[1]);
        Eigen::Map<Eigen::Vector3d> residual_map(residual);
        
        residual_map = Sophus::SO3d::exp(so3_map)*watched_point_+translation_map -point_in_world_;

        if (!jacobians) return true;
        double* jacobian = jacobians[0];
        if (!jacobian) return true;
        Eigen::Matrix<double,3,3> jacobian_R = Eigen::Matrix<double,3,3>::Identity();
        jacobian_R = -1*Sophus::SO3d::exp(so3_map).matrix()*Sophus::SO3d::hat(watched_point_)*
                     RightJacobianSO3(so3_map.x(),so3_map.y(),so3_map.z()).inverse();
        Eigen::Matrix<double,3,3> jacobian_t = Eigen::Matrix<double,3,3>::Identity();
        
        int count = 0;
        for(int i=0;i<3;i++)
        {
            for(int j=0;j<3;j++)
            {
                jacobians[0][count] = jacobian_R(i,j);
                count++;
            }
        }
        count = 0;
        for(int i=0;i<3;i++)
        {
            for(int j=0;j<3;j++)
            {
                jacobians[1][count] = jacobian_t(i,j);
                count++;
            }
        }   
        return true;
    }

    static ceres::CostFunction * Create(const Eigen::Vector3d watched_point, 
                                        const Eigen::Vector3d point_in_world)
    {
        return new SO3R3ICPErrorNoneDelta(watched_point,point_in_world);
    }
    private:
    Eigen::Vector3d watched_point_;
    Eigen::Vector3d point_in_world_;
};



int main()
{
    buildPointCloud();
    const double transform_lie [6] {0.4,0.6,0.7,0.6,0.7,0.8};
    Eigen::Map<const Eigen::Matrix<double,6,1>> transform_map(transform_lie);
    Sophus::SE3<double> transform_se3 = Sophus::SE3d::exp(transform_map);

    std::vector<Eigen::Vector4d> point_in_camera;
    transformPointCloud(transform_se3,point_cloud,point_in_camera);
    AddNoise(point_in_camera);

    cout<<" show the transform matrix from SE3"<<endl
        <<transform_se3.matrix()<<endl;
    
    assert(point_in_camera.front()(3) == 1);
    assert(point_in_camera.back()(3) == 1);    

    ceres::Problem problem;

    // 之前这里忘记使用exp 对se3 进行变换了。
    // 确很巧合地对比出了 SE3 相对于 Quternion + R3 惊人的稳定性
    double initial_lie [6] {0.44,0.65,0.72,0.55,0.73,0.86};
    Eigen::Map<Eigen::Matrix<double,6,1>> init_map(initial_lie);
    
    // 现在这么看的的确是节省了一些添加 LocalParameterization 的代码
    problem.AddParameterBlock(init_map.data(),3,new SO3PlusOnlyLocalParameterization());
    problem.AddParameterBlock(init_map.data()+3,3);
    for(int i = 0; i<point_in_camera.size();i++)
    {
        ceres::CostFunction * cf = SO3R3ICPError::Create(point_in_camera[i].segment(0,3),point_cloud[i].segment(0,3));

        problem.AddResidualBlock(cf,new ceres::HuberLoss(0.1),init_map.data(),init_map.data()+3);       
    }

    // problem.AddParameterBlock(init_map.data(),3);
    // problem.AddParameterBlock(init_map.data()+3,3);
    // for(int i = 0; i<point_in_camera.size();i++)
    // {
    //     ceres::CostFunction * cf = SO3R3ICPErrorNoneDelta::Create(point_in_camera[i].segment(0,3),point_cloud[i].segment(0,3));

    //     problem.AddResidualBlock(cf,new ceres::HuberLoss(0.1),init_map.data(),init_map.data()+3);       
    // }

    ceres::Solver::Options options;
    ceres::Solver::Summary summary;
    options.linear_solver_type = ceres::DENSE_QR;
    options.minimizer_type = ceres::TRUST_REGION;
    options.trust_region_strategy_type = ceres::LEVENBERG_MARQUARDT;
    options.minimizer_progress_to_stdout = true;
    // options.max_num_iterations =20;

    // 这个Solve 和那个不能用的solve 看起来还是有区分度的
    ceres::Solve(options,&problem,&summary);

    cout<<summary.BriefReport()<<endl;
    cout<<"the translation matrix is "<<std::endl<<init_map.transpose()<<endl;
    return 0;
}

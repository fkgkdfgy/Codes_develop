/*
 * @Author: Liu Weilong
 * @Date: 2020-11-02 07:59:53
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2020-11-29 21:21:23
 * @Description: 
 *                Auto Diff Quternion + R3 上的优化 
 *                ceres 的 EigenQuternionLocalParameterization 
 *                能够让人非常简单地进行求导
 *                但是 SE3 的稳定性真的太惊人了
 * 
 *                这里提供是 T*p 的Quternion + R3 的优化模型
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


class QuternionR3ICPError
{
    public:
    QuternionR3ICPError(const Eigen::Vector3d watched_point, const Eigen::Vector3d point_in_world):
                         watched_point_(watched_point),point_in_world_(point_in_world){}

    template <typename T>
    bool operator()(const T * quternion, const T* translation , T *residual)const
    {
        Eigen::Map<Eigen::Matrix<T,3,1>> resiudal_map(residual);
        Eigen::Map<const Eigen::Quaternion<T>> quternion_map(quternion);
        Eigen::Map<const Eigen::Matrix<T,3,1>> translation_map(translation);
        

        // Eigen::Quaternion<T> quternion_map(quternion[3],quternion[0],quternion[1],quternion[2]);
        // 果然对于Eigen来说 不能 Matrix<Jet,3,3> * Matrix<double ,3,1> 相乘
        // 需要把Matrix<double,3,1> 转换成 Matrix<Jet,3,1>

        Eigen::Matrix<T,3,1> watched_point_T;
        watched_point_T << T(watched_point_(0)),T(watched_point_(1)),T(watched_point_(2));

        Eigen::Matrix<T,3,1> point_in_world_T;
        point_in_world_T << T(point_in_world_(0)),T(point_in_world_(1)),T(point_in_world_(2));

        resiudal_map = quternion_map * watched_point_T + translation_map -point_in_world_T;
        return true;
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
    Eigen::Map<const Eigen::Matrix<double,6,1>> init_map(initial_lie);
    Sophus::SE3<double> show_lie = Sophus::SE3d::exp(init_map);

    Eigen::Quaterniond quternion(show_lie.rotationMatrix());
    Eigen::Translation3d translation(show_lie.translation());

    
    cout<<" the init SE3 is "<<endl
        << show_lie.matrix()<<endl;
    // 现在这么看的的确是节省了一些添加 LocalParameterization 的代码
    problem.AddParameterBlock(quternion.coeffs().data(),4,new ceres::EigenQuaternionParameterization());
    problem.AddParameterBlock(translation.translation().data(),3);
    for(int i = 0; i<point_in_camera.size();i++)
    {
        ceres::CostFunction * cf = new ceres::AutoDiffCostFunction<QuternionR3ICPError,3,4,3>(
                                       new QuternionR3ICPError(point_in_camera[i].segment(0,3),point_cloud[i].segment(0,3)));
        problem.AddResidualBlock(cf,new ceres::HuberLoss(0.1),quternion.coeffs().data(),translation.translation().data());       
    }
   

    ceres::Solver::Options options;
    ceres::Solver::Summary summary;
    options.linear_solver_type = ceres::DENSE_QR;
    options.minimizer_type = ceres::TRUST_REGION;
    options.trust_region_strategy_type = ceres::LEVENBERG_MARQUARDT;
    options.minimizer_progress_to_stdout = true;

    // 这个Solve 和那个不能用的solve 看起来还是有区分度的
    ceres::Solve(options,&problem,&summary);

    cout<<summary.BriefReport()<<endl;
    cout<<"the rotation matrix is "<<std::endl<< quternion.toRotationMatrix()<<endl
        <<"the translation  is "<<std::endl<< translation.translation().transpose()<<endl;
    
    cout<<"the right se3 is "<<endl<<transform_se3.matrix3x4()<<endl;
    
};

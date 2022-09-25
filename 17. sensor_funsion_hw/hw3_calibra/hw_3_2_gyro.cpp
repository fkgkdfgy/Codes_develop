/*
 * @Author: Liu Weilong
 * @Date: 2020-11-08 10:28:52
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2020-12-02 07:28:35
 * @Description: 代码用于求解分立式标定
 *               代码分成两个部分一个IMU_accel
 *                                IMU_gyro
 *      增加一个积分环节 虽然结果可以但是感觉这里做的有问题这里并不是一个SO3上的积分
     虽然不是 但是 因为数值比较小，所以采用泰勒一阶展开结果也还行
    2020.12.3 这个结果目前感觉还是有一些问题的之后需要具体看一下
 */
#include <iostream>
#include <ceres/ceres.h>
#include <Eigen/Eigen>
#include <omp.h>
#include "read_csv.h"

using namespace std;

Eigen::Vector3d axis(1,0.3,0.4);
Eigen::AngleAxisd angle_axis(0.05,axis.normalized());
Eigen::Matrix3d error_matrix = angle_axis.matrix();

template<typename T>
bool buildErrorMatrix(const T * k, const T * s,
                        Eigen::Matrix<T,3,3> & error_matrix) 
{
    error_matrix<<k[0],s[0],s[1]
                 ,s[2],k[1],s[3]
                 ,s[4],s[5],k[2];
    return true;
}
// 函数同样适应于 ImuGyro 的误差模型的计算
class ImuAccelError 
{
    public:
    ImuAccelError(Eigen::Vector3d measurement,Eigen::Vector3d real):
                  measurement_(measurement),real_(real){}

    template<typename T>
    bool operator()(const T * K, const T * S,const T* B, T* residual) const
    {
        Eigen::Map<const Eigen::Matrix<T,3,1>> B_map(B);
        Eigen::Map<Eigen::Matrix<T,3,1>> residual_map(residual);
        Eigen::Matrix<T,3,3> error_matrix ;
        buildErrorMatrix(K,S,error_matrix);

        residual_map = measurement_.cast<T>()- error_matrix * real_.cast<T>()-B_map;
        return true;
    }


    static ceres::CostFunction * CreateError(Eigen::Vector3d measurement,Eigen::Vector3d real)
    {
        return new ceres::AutoDiffCostFunction<ImuAccelError,3,3,6,3>
                                   (new ImuAccelError(measurement,real));
    }
    Eigen::Vector3d measurement_;
    Eigen::Vector3d real_;
};

bool GyroIntegration(std::vector<Eigen::Matrix<double,3,1>> & angular_velocity, 
                     Eigen::Vector3d & integration_result,
                     double time_interval,int n)
{
    if(angular_velocity.size()==0 || time_interval<0 || n<0)
    {
        cout<<"the angular_velocity's size or time_interval may be wrong !!"<<endl;
        return false;
    }

    omp_set_num_threads(6);
    double integration_x = 0,integration_y=0,integration_z = 0;
    #pragma omp parallel for reduction(+:integration_x,integration_y,integration_z)
    for(int i = 0;i<n;i++)
    {
        integration_x += angular_velocity[i].x()*time_interval;
        integration_y += angular_velocity[i].y()*time_interval;
        integration_z += angular_velocity[i].z()*time_interval;
    }
    
    integration_result.x() = integration_x;
    integration_result.y() = integration_y;
    integration_result.z() = integration_z;
    
    return true;
}

int main()
{
    // 设置刻度误差，安装误差在定义的时候已经定义好了。
    error_matrix(0,0) = 1.02;
    error_matrix(1,1) = 0.88;
    error_matrix(2,2) = 1.10;

    std::cout<<"show the target error model is "<<endl
             << error_matrix<<endl;

    // readCSV 数据
    std::string measure_path="/home/lwl/workspace/HW/gnss-ins-sim/demo_motion_def_files/imu_simulation_hw/gyro-0.csv";
    std::string real_path ="/home/lwl/workspace/HW/gnss-ins-sim/demo_motion_def_files/imu_simulation_hw/ref_gyro.csv";
    std::string time_csv_path="/home/lwl/workspace/HW/gnss-ins-sim/demo_motion_def_files/imu_simulation_hw/time.csv";
    std::vector<string> path_array;
    path_array.push_back(measure_path);
    path_array.push_back(real_path);
    path_array.push_back(time_csv_path);
    std::vector<vector<vector<double>>> data_array;
    std::vector<vector<std::string>> headers_array;
    headers_array.resize(path_array.size());
    data_array.resize(path_array.size());

    omp_set_num_threads(3);
    #pragma omp parallel for
    for(int i =0;i<path_array.size();i++)
    {
        readCSV(headers_array.at(i),data_array.at(i),path_array.at(i));
        
    }

    auto & accel_measure_data = data_array.at(0);
    auto & real_accel_data = data_array.at(1);
    std::vector<Eigen::Matrix<double,3,1>> new_gyro_measure ;

    // 因为gnss-ins-sim 没有对刻度和安装误差进行考虑 所以 real + bias + noise = measurement
    // 这里做的是让 error*real +bias + noise = new_measurement 
    new_gyro_measure.reserve(accel_measure_data.front().size());

    for(int i=0;i<accel_measure_data.front().size();i++)
    {
        Eigen::Matrix<double,3,1> tmp_measure(accel_measure_data.at(0)[i],
                                              accel_measure_data.at(1)[i],
                                              accel_measure_data.at(2)[i]);
        Eigen::Matrix<double,3,1> tmp_real(real_accel_data.at(0)[i],
                                           real_accel_data.at(1)[i],
                                           real_accel_data.at(2)[i]);
        auto bias_and_noise = tmp_measure - tmp_real;
        new_gyro_measure.push_back(error_matrix*tmp_real + bias_and_noise);
    }


    double interval = 1.0/100.0;
    
    std::vector<Eigen::Vector3d> gyro_integration;
    gyro_integration.resize(new_gyro_measure.size(),Eigen::Vector3d::Zero());
    for(int i=1;i<new_gyro_measure.size();i++)
    {
        gyro_integration[i] = gyro_integration[i-1] + new_gyro_measure[i-1] * interval;
    }
    std::vector<Eigen::Vector3d> gyro_integration_without_noise;
    gyro_integration_without_noise.resize(new_gyro_measure.size(),Eigen::Vector3d::Zero());
    for(int i=1;i<new_gyro_measure.size();i++)
    {
        Eigen::Matrix<double,3,1> tmp_real(real_accel_data.at(0)[i-1],
                                           real_accel_data.at(1)[i-1],
                                           real_accel_data.at(2)[i-1]);
        gyro_integration_without_noise[i] = gyro_integration_without_noise[i-1] + tmp_real * interval;
    }  

    // 数据 下采样一下然后丢进 ceres 进行优化

    ceres::Problem problem;
    double k[3]{0,0,0};
    double s[6]{0,0,0,0,0,0};
    double b[3]{0,0,0};
    Eigen::Map<Eigen::Matrix<double,3,1>> k_map(k);
    Eigen::Map<Eigen::Matrix<double,6,1>> s_map(s);
    Eigen::Map<Eigen::Matrix<double,3,1>> b_map(b);

    problem.AddParameterBlock(k,3);
    problem.AddParameterBlock(s,6);
    problem.AddParameterBlock(b,3);
    for(int i=0;(10*i)<gyro_integration_without_noise.size();i++)
    {
        problem.AddResidualBlock(ImuAccelError::CreateError(gyro_integration[i*10],gyro_integration_without_noise[i*10]),new ceres::CauchyLoss(0.1),k,s,b);
    }
    
    ceres::Solver::Options options;
    ceres::Solver::Summary summary;
    options.linear_solver_type = ceres::DENSE_QR;
    options.minimizer_type = ceres::TRUST_REGION;
    options.trust_region_strategy_type = ceres::LEVENBERG_MARQUARDT;
    options.minimizer_progress_to_stdout = true;

    ceres::Solve(options,&problem,&summary);
    cout<<summary.BriefReport()<<endl;
    
    Eigen::Matrix<double,3,3> estimated_error_matrix;
    buildErrorMatrix(k,s,estimated_error_matrix);
    cout<<"the estimation error matrix is "<< endl<<estimated_error_matrix<<endl;
    cout<<"the estimation bias is "<<endl<<b_map.transpose()<<endl;
    
}
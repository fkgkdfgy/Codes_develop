/*
 * @Author: Liu Weilong
 * @Date: 2020-11-08 10:28:52
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2020-11-08 22:14:56
 * @Description: 代码用于求解分立式标定
 *               代码分成两个部分一个IMU_accel
 *                                IMU_gyro
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


int main()
{
    // 设置刻度误差，安装误差在定义的时候已经定义好了。
    error_matrix(0,0) = 1.02;
    error_matrix(1,1) = 0.88;
    error_matrix(2,2) = 1.10;

    std::cout<<"show the target error model is "<<endl
             << error_matrix<<endl;

    // readCSV 数据
    std::string accel_measure_path="/home/lwl/workspace/HW/gnss-ins-sim/demo_motion_def_files/imu_simulation_hw/accel-0.csv";
    std::string real_accel_path ="/home/lwl/workspace/HW/gnss-ins-sim/demo_motion_def_files/imu_simulation_hw/ref_accel.csv";
    std::string time_csv_path="/home/lwl/workspace/HW/gnss-ins-sim/demo_motion_def_files/imu_simulation_hw/time.csv";
    std::vector<string> path_array;
    path_array.push_back(accel_measure_path);
    path_array.push_back(real_accel_path);
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
    std::vector<Eigen::Matrix<double,3,1>> new_accel_measure ;

    // 因为gnss-ins-sim 没有对刻度和安装误差进行考虑 所以 real + bias + noise = measurement
    // 这里做的是让 error*real +bias + noise = new_measurement 
    new_accel_measure.reserve(accel_measure_data.front().size());

    for(int i=0;i<accel_measure_data.front().size();i++)
    {
        Eigen::Matrix<double,3,1> tmp_measure(accel_measure_data.at(0)[i],
                                              accel_measure_data.at(1)[i],
                                              accel_measure_data.at(2)[i]);
        Eigen::Matrix<double,3,1> tmp_real(real_accel_data.at(0)[i],
                                           real_accel_data.at(1)[i],
                                           real_accel_data.at(2)[i]);
        auto bias_and_noise = tmp_measure - tmp_real;
        new_accel_measure.push_back(error_matrix*tmp_real + bias_and_noise);
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
    for(int i=0;(10*i)<new_accel_measure.size();i++)
    {
        Eigen::Matrix<double,3,1> tmp_real(real_accel_data.at(0)[i*10],
                                           real_accel_data.at(1)[i*10],
                                           real_accel_data.at(2)[i*10]);
        problem.AddResidualBlock(ImuAccelError::CreateError(new_accel_measure[i*10],tmp_real),new ceres::CauchyLoss(0.1),k,s,b);
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
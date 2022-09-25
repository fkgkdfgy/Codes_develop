/*
 * @Author: Liu Weilong
 * @Date: 2020-11-09 23:06:06
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2020-11-30 20:51:39
 * @Description: 半系统级 imu 标定
 *               先是gyro 的标定 1. 造误差数据 debug 没有问题
 *                             2. 找间隔     debug 没有问题
 *                             4. 积分的问题  目前看起来是有问题的 之前的内容太暴力 已经更改成细腻一点的版本
 *                             3. 做优化     目前怀疑是优化有问题
 *               半系统级 accel 标定
 *               
 *  
 *               2020.11.30 当时做的时候仅仅是对 accel 的 KS Bias 和
 *                                            gyro 的KS  进行标定
 *                                            但是没有标定 gyro的bias
 *                          之后应该重新写一下
 */
#include "read_csv.h"
#include "sophus/so3.hpp"
#include "Eigen/Eigen"
#include "ceres/ceres.h"
#include <utility>
#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
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

// 这里是SO3 的泰勒一阶展开
template<typename T>
void integrateGyroMeasurementEuler(Eigen::Matrix<T,3,3> & K_and_S,
                                   std::vector<Eigen::Vector3d> gyro_motion_measurements,
                                   Eigen::Matrix<T,3,3> & integrated_result,
                                   T deltaT)
{
    Eigen::Matrix<T,3,1> sum = Eigen::Matrix<T,3,1>::Zero();
    integrated_result = Eigen::Matrix<T,3,3>::Identity();
    for(auto & measurement:gyro_motion_measurements)
    {
        auto calibed_gyro_info = K_and_S* measurement.cast<T>();
        integrated_result = integrated_result * (Eigen::Matrix3d::Identity()+ Sophus::SO3<T>::hat(calibed_gyro_info/180.0*M_PI)*deltaT);
    }
}

template<typename T>
void integrateGyroMeasurementEuler(Eigen::Matrix<T,3,3> & K_and_S,
                                   Eigen::Matrix<T,3,1> & Bias,
                                   std::vector<Eigen::Vector3d> gyro_motion_measurements,
                                   Eigen::Matrix<T,3,3> & integrated_result,
                                   T deltaT)
{
    Eigen::Matrix<T,3,1> sum = Eigen::Matrix<T,3,1>::Zero();
    integrated_result = Eigen::Matrix<T,3,3>::Identity();
    for(auto & measurement:gyro_motion_measurements)
    {
        auto calibed_gyro_info = K_and_S*(measurement.cast<T>()-Bias);
        integrated_result = integrated_result * (Eigen::Matrix3d::Identity()+ Sophus::SO3<T>::hat(calibed_gyro_info/180.0*M_PI)*deltaT);
    }
}


void transformIntoVector(const std::vector<std::vector<double>> & data, std::vector<Eigen::Vector3d> & output)
{
    output.clear();
    output.reserve(data.front().size());
    for(int i=0;i<data.front().size();i++)
    {
        output.push_back(Eigen::Vector3d(data.at(0)[i],data.at(1)[i],data.at(2)[i]));
    }

}


void showVector(std::vector<Eigen::Vector3d> & tmp,unsigned int num)
{
    int count =0;
    for(auto & element:tmp)
    {
        std::cout<<element.transpose()<<endl;
        count++;
        if(count == num)
        {return;}
    }
}

auto IntegrationUsedInSemiCalib(const vector<Eigen::Vector3d> & gyro_measurements,
                         double dt)
{
    Sophus::SO3d cur = Sophus::SO3d::exp(Eigen::Vector3d::Zero());
    Eigen::Matrix3d cur_matrix = cur.matrix();
    
    Eigen::Vector3d sum = Eigen::Vector3d::Zero();
    for(auto & measurement:gyro_measurements)
    {
        // cur_matrix = cur_matrix*(Eigen::Matrix3d::Identity()+ Sophus::SO3d::hat(error_matrix*measurement/180.0*M_PI)*dt);
        cur_matrix = cur_matrix*(Eigen::Matrix3d::Identity()+ Sophus::SO3d::hat(measurement/180.0*M_PI)*dt);
    }
    
    return cur_matrix;
}

class CalibAccel
{
    public:
    CalibAccel(std::vector<Eigen::Vector3d> * accel_data_ptr,
               std::vector<Eigen::Vector3d> * gyro_data_real_ptr,
               const double kmgravity):
               accel_data_ptr_(accel_data_ptr),
               gyro_data_real_ptr_(gyro_data_real_ptr),
               kmgravity_(kmgravity){}
    
    void buildProblem()
    {
        ceres::Problem problem;
        double K[3]{0.5,0.5,0.5};
        double S[6]{0.1,-0.1,0.1,0.0,0.0,0.0};
        // double K[3]{0.1,0.1,0.1};
        // double S[6]{0.1,0.1,0.1,0.1,0.1,0.1};ss
        double Bias[3]{0.0,0.0,0.0};
        problem.AddParameterBlock(K,3);
        problem.AddParameterBlock(S,3);
        problem.AddParameterBlock(Bias,3);
        // problem.SetParameterBlockConstant(Bias);
        unsigned int pre_index= 0;
        // 用于寻找motion_interval

        // ========================= 添加测试变量 =============================

        unsigned int tmp_index = 0;
        bool motion_interval = false;

        for(int i =0;i< gyro_data_real_ptr_->size();i++)
        {
            if(!motion_interval)
            {
                if(gyro_data_real_ptr_->at(i).norm()!=0.0)
                {
                    motion_interval = true;
                    pre_index=i;
                }
            }
            else
            {
                if(gyro_data_real_ptr_->at(i).norm()==0.0)
                {
                    motion_interval = false;
                    
                    ceres::CostFunction * cf = AccelCostFunctor::Create(kmgravity_,accel_data_ptr_->at(i));
                    // showVector(gyro_in_motion_interval,10);
                    problem.AddResidualBlock(cf,NULL,K,S,Bias);
                    
                    continue;
                }
            }  
        }    
        
        ceres::Solver::Options options;
        ceres::Solver::Summary summary;
        options.linear_solver_type = ceres::DENSE_QR;
        options.minimizer_type = ceres::TRUST_REGION;
        options.trust_region_strategy_type = ceres::LEVENBERG_MARQUARDT;
        options.minimizer_progress_to_stdout = true;
        
        ceres::Solve(options,&problem,&summary);
        cout<<summary.BriefReport()<<endl;

        K_ = Eigen::Map<Eigen::Vector3d>(K);
        S_ = Eigen::Map<Eigen::Matrix<double,3,1>>(S);
        Bias_ = Eigen::Map<Eigen::Vector3d>(Bias); 
        
        Eigen::Matrix<double,6,1> S_6;
        S_6<<0,0,S_(0),0,S_(1),S_(2);
        
        buildErrorMatrix(K_.data(),S_6.data(),estimated_error_matrix_);
        cout<<"show the estimation error matrix is "<< endl
            <<estimated_error_matrix_<<endl;

    }

    auto getErrorMatrix(){return estimated_error_matrix_;}
    auto getBias(){return Bias_;}
    private:
    std::vector<Eigen::Vector3d> * accel_data_ptr_;
    std::vector<Eigen::Vector3d> * gyro_data_real_ptr_;
    Eigen::Matrix<double,3,1> K_ = Eigen::Vector3d::Zero();
    Eigen::Matrix<double,3,1> S_ = Eigen::Vector3d::Zero();
    Eigen::Matrix<double,3,3> estimated_error_matrix_;
    Eigen::Matrix<double,3,1> Bias_ = Eigen::Vector3d::Zero();

    double kmgravity_;

    class AccelCostFunctor
    {
        public:
        
        AccelCostFunctor(const double kmgravity, const Eigen::Vector3d accel_vector):
                         kmgravity_(kmgravity),accel_vector_(accel_vector){}

        template<typename T>
        bool operator()(const T*K, const T *S, const T* Bias, T* residual) const
        {
            Eigen::Matrix<T,3,3,Eigen::RowMajor> error_matrix_T;
            error_matrix_T<<K[0],T(0.0),T(0.0)
                         ,S[0],K[1],T(0.0)
                         ,S[1],S[2],K[2];
            Eigen::Map<const Eigen::Matrix<T,3,1>> Bias_map(Bias);
            Eigen::Matrix<T,3,1> new_gravity = error_matrix_T* (accel_vector_.cast<T>()-Bias_map);
            residual[0] = T(kmgravity_) - new_gravity.norm();
            return true;
        }

        static ceres::CostFunction * Create(const double kmgravity, const Eigen::Vector3d accel_vector)
        {
            return new ceres::AutoDiffCostFunction<AccelCostFunctor,1,3,3,3>(
                       new AccelCostFunctor(kmgravity,accel_vector));
        }
        private:
        const double kmgravity_;
        const Eigen::Vector3d accel_vector_;
    };

};

class CalibGyro
{
    public:
    CalibGyro(std::vector<Eigen::Vector3d> * gyro_data_ptr,
              std::vector<Eigen::Vector3d> * gyro_data_real_ptr,
              std::vector<Eigen::Vector3d> * corresponding_gravity_ptr,
              double hz):
              gyro_motion_array_(gyro_data_ptr),
              gyro_motion_real_(gyro_data_real_ptr),
              corresponding_gravity_array_(corresponding_gravity_ptr),
              hz_(hz){
                  real_RPY_.emplace_back(0.0,20.0,66.0);
                  real_RPY_.emplace_back(0.0,40.0,0.0);
                  real_RPY_.emplace_back(0.0,0.0,40.0);
                  real_RPY_.emplace_back(23.0,45.0,60.0);
                  real_RPY_.emplace_back(77.0,44.0,6.0);
              }
    
    void buildProblem()
    {
        ceres::Problem problem;
        bool motion_interval = false;
        // double K[3]{1.02,0.88,1.11};
        // double S[6]{-0.01758,0.0138,0.0181,-0.0445828,-0.013,0.04482};

        double K[3]{0.1,0.1,0.1};
        double S[6]{0.1,0.1,0.1,0.1,0.1,0.1};
        problem.AddParameterBlock(K,3);
        problem.AddParameterBlock(S,6);
        std::vector<Eigen::Vector3d> gyro_in_motion_interval;
        unsigned int pre_index= 0;
        // 用于寻找motion_interval

        // ========================= 添加测试变量 =============================

        unsigned int tmp_index = 0;


        for(int i =0;i< gyro_motion_real_->size();i++)
        {
            if(!motion_interval)
            {
                if(gyro_motion_real_->at(i).norm()!=0.0)
                {
                    motion_interval = true;
                    pre_index=i;
                    gyro_in_motion_interval.push_back(gyro_motion_array_->at(i));
                }
            }
            else
            {
                if(gyro_motion_real_->at(i).norm()==0.0)
                {
                    motion_interval = false;
                    ceres::CostFunction * cf = GyroCostFunctor::Create(gyro_in_motion_interval,
                                                                       corresponding_gravity_array_->at(pre_index),
                                                                       corresponding_gravity_array_->at(i),
                                                                       1.0/hz_);
                    // showVector(gyro_in_motion_interval,10);
                    problem.AddResidualBlock(cf,NULL,K,S);
                    
                    // ================== 测试 ===========================

                    // Eigen::Matrix3d rotation_matrix1;
                    // rotation_matrix1 = Eigen::AngleAxisd(real_RPY_[tmp_index].x()/180.0*M_PI, Eigen::Vector3d::UnitZ()) *
                    //             Eigen::AngleAxisd(real_RPY_[tmp_index].y()/180.0*M_PI, Eigen::Vector3d::UnitY()) *
                    //             Eigen::AngleAxisd(real_RPY_[tmp_index].z()/180.0*M_PI, Eigen::Vector3d::UnitX());
                    
                    // cout<<" the real matrix is"<<endl
                    //     << rotation_matrix1<<endl;
                    // tmp_index++;
                    // static Eigen::Matrix<double,3,3> acculu_matrix = Eigen::Matrix3d::Identity();
                    // auto integration_matrix = IntegrationUsedInSemiCalib(gyro_in_motion_interval,1.0/hz_);
                    
                    // cout<<"the integration matrix is "<<endl
                    //     << acculu_matrix* integration_matrix<<endl;
                    // acculu_matrix =  acculu_matrix* integration_matrix;
                    // std::cout<<" the current error is " <<std::endl
                    //          << (integration_matrix
                    //              *corresponding_gravity_array_->at(pre_index)-
                    //              corresponding_gravity_array_->at(i)).transpose()<<std::endl;
                                 
                    // cout<<"the origin gravity vector is "<<endl
                    //     <<corresponding_gravity_array_->at(pre_index).transpose()<<endl;
                    // cout<<"the final gravity is "<<endl
                    //     <<corresponding_gravity_array_->at(i).transpose()<<endl;
                    
                    // cout<<"the transformed final gravity is "<<endl
                    //     <<integration_matrix* corresponding_gravity_array_->at(i)<<endl;

                    gyro_in_motion_interval.clear();
                    continue;
                }
                gyro_in_motion_interval.push_back(gyro_motion_array_->at(i));
            }  
        }    
        
        ceres::Solver::Options options;
        ceres::Solver::Summary summary;
        options.linear_solver_type = ceres::DENSE_QR;
        options.minimizer_type = ceres::TRUST_REGION;
        options.trust_region_strategy_type = ceres::LEVENBERG_MARQUARDT;
        options.minimizer_progress_to_stdout = true;
        
        ceres::Solve(options,&problem,&summary);
        cout<<summary.BriefReport()<<endl;

        K_ = Eigen::Map<Eigen::Vector3d>(K);
        S_ = Eigen::Map<Eigen::Matrix<double,6,1>>(S);

    }

    const auto getResult(){
        return std::tuple<Eigen::Matrix<double,3,1>,Eigen::Matrix<double,6,1>>(K_,S_); 
    }
    const auto getErrorMatrix()
    {
        Eigen::Matrix3d result;
        buildErrorMatrix(K_.data(),S_.data(),result);
        return result;
    }
    
    private:
    std::vector<Eigen::Vector3d> * gyro_motion_array_;
    std::vector<Eigen::Vector3d> * gyro_motion_real_;
    std::vector<Eigen::Vector3d> * corresponding_gravity_array_;
    std::vector<Eigen::Vector3d>   real_RPY_;
    double hz_;
    Eigen::Matrix<double,3,1> K_;
    Eigen::Matrix<double,6,1> S_;
    Eigen::Matrix<double,3,1> Bias_ = Eigen::Vector3d::Zero();

    class GyroCostFunctor
    {
        public:
        GyroCostFunctor(std::vector<Eigen::Vector3d>  gyro_motion_measurment,
                        Eigen::Vector3d corresponding_gravity_front,
                        Eigen::Vector3d corresponding_gravity_back,
                        double dt):
                        gyro_motion_measurement_(gyro_motion_measurment),
                        corresponding_gravity_front_(corresponding_gravity_front),
                        corresponding_gravity_back_(corresponding_gravity_back),
                        deltaT(dt){}

        template<typename T>
        bool operator()(const T* K, const T* S, T* residual)const
        {
            Eigen::Matrix<T,3,3> error_model;
            buildErrorMatrix(K,S,error_model);
            Eigen::Matrix<T,3,3> integration;
            integrateGyroMeasurementEuler(error_model,gyro_motion_measurement_,integration,T(deltaT));

            Eigen::Map<Eigen::Matrix<T,3,1>> residual_map(residual);
            residual_map = corresponding_gravity_front_.cast<T>()-integration*corresponding_gravity_back_.cast<T>();
            return true;
        }
        
        static ceres::CostFunction * Create(std::vector<Eigen::Vector3d>  gyro_motion_measurment,
                                            Eigen::Vector3d corresponding_gravity_front,
                                            Eigen::Vector3d corresponding_gravity_back,
                                            double dt)
        {
            return new ceres::AutoDiffCostFunction<GyroCostFunctor,3,3,6>
                       (new GyroCostFunctor(gyro_motion_measurment,
                                            corresponding_gravity_front,
                                            corresponding_gravity_back,
                                            dt));
        }

        private:
        std::vector<Eigen::Vector3d>  gyro_motion_measurement_;
        Eigen::Vector3d corresponding_gravity_front_;
        Eigen::Vector3d corresponding_gravity_back_;
        double deltaT;
    };
    class GyroWithBiasCostFunctor
    {
        public:
        GyroWithBiasCostFunctor(std::vector<Eigen::Vector3d>  gyro_motion_measurment,
                                Eigen::Vector3d corresponding_gravity_front,
                                Eigen::Vector3d corresponding_gravity_back,
                                double dt):
                                gyro_motion_measurement_(gyro_motion_measurment),
                                corresponding_gravity_front_(corresponding_gravity_front),
                                corresponding_gravity_back_(corresponding_gravity_back),
                                deltaT(dt){}

        template<typename T>
        bool operator()(const T* K, const T* S,const T * Bias, T* residual)const
        {
            Eigen::Matrix<T,3,3> error_model;
            buildErrorMatrix(K,S,error_model);
            Eigen::Matrix<T,3,3> integration;
            Eigen::Map<Eigen::Matrix<T,3,1>> Bias_map(Bias);
            integrateGyroMeasurementEuler(error_model,Bias_map,gyro_motion_measurement_,integration,T(deltaT));

            Eigen::Map<Eigen::Matrix<T,3,1>> residual_map(residual);
            residual_map = corresponding_gravity_front_.cast<T>()-integration*corresponding_gravity_back_.cast<T>();
            return true;
        }
        
        static ceres::CostFunction * Create(std::vector<Eigen::Vector3d>  gyro_motion_measurment,
                                            Eigen::Vector3d corresponding_gravity_front,
                                            Eigen::Vector3d corresponding_gravity_back,
                                            double dt)
        {
            return new ceres::AutoDiffCostFunction<GyroCostFunctor,3,3,6>
                       (new GyroCostFunctor(gyro_motion_measurment,
                                            corresponding_gravity_front,
                                            corresponding_gravity_back,
                                            dt));
        }

        private:
        std::vector<Eigen::Vector3d>  gyro_motion_measurement_;
        Eigen::Vector3d corresponding_gravity_front_;
        Eigen::Vector3d corresponding_gravity_back_;
        double deltaT;
    };
};

int main()
{
    // ==================================== read_csv =================================================
    
    std::string gyro_real_measurement = "/home/lwl/workspace/HW/gnss-ins-sim/demo_motion_def_files/imu_simulation_hw1/ref_gyro.csv";
    std::string gyro_measurement = "/home/lwl/workspace/HW/gnss-ins-sim/demo_motion_def_files/imu_simulation_hw1/gyro-0.csv";
    std::string gravity_measurement = "/home/lwl/workspace/HW/gnss-ins-sim/demo_motion_def_files/imu_simulation_hw1/ref_accel.csv";
    std::string accel_measurement = "/home/lwl/workspace/HW/gnss-ins-sim/demo_motion_def_files/imu_simulation_hw1/accel-0.csv";
    std::vector<std::string> headers_real;
    std::vector<std::vector<double>> data_real;
    std::vector<std::string> headers;
    std::vector<std::vector<double>> data;
    std::vector<std::string> headers_gravity;
    std::vector<std::vector<double>> data_gravity;
    std::vector<std::string> headers_accel;
    std::vector<std::vector<double>> accel_data;

    std::vector<Eigen::Vector3d> gyro_in_vector;
    std::vector<Eigen::Vector3d> gyro_real_in_vector;
    std::vector<Eigen::Vector3d> accel_in_vector;
    std::vector<Eigen::Vector3d> gravity_in_vector;
    
    if(!readCSV(headers_real,data_real,gyro_real_measurement))
    {
        std::abort();
    };
    if(!readCSV(headers,data,gyro_measurement))
    {
        std::abort();
    };
    if(!readCSV(headers_gravity,data_gravity,gravity_measurement))
    {
        std::abort();
    };
    if(!readCSV(headers_accel,accel_data,accel_measurement))
    {
        std::abort();
    };

    transformIntoVector(data_real,gyro_real_in_vector);
    transformIntoVector(data,gyro_in_vector);
    transformIntoVector(data_gravity,gravity_in_vector);
    transformIntoVector(accel_data,accel_in_vector);

    // ================================== 添加 安装误差 gyro ==========================================
    
    error_matrix(0,0) = 1.02;
    error_matrix(1,1) = 0.88;
    error_matrix(2,2) = 1.10;

    std::cout<<"show the target error model is "<<endl
             << error_matrix<<endl;
    
    Eigen::Matrix3d error_matrix_inv = error_matrix.inverse();

    std::cout<<"show error matrix inverse"<<endl
             << error_matrix_inv<<endl;

    std::cout<<"the origin gyro data"<<endl;
    // showVector(gyro_in_vector,10);
    
    for(auto & info:gyro_in_vector)
    {
        info = error_matrix_inv * info;
    }

    // ================================= 添加 安装误差 accel =========================================

    Eigen::Matrix3d error_matrix_accel = Eigen::Matrix3d::Identity();
    error_matrix_accel(0,0) = 1.02;
    error_matrix_accel(1,1) = 0.88;
    error_matrix_accel(2,2) = 1.10;

    error_matrix_accel(1,0) = error_matrix(1,0);
    error_matrix_accel(2,0) = error_matrix(2,0);
    error_matrix_accel(2,1) = error_matrix(2,1);
    
    cout<<"show the accel's error matrix"<<endl
        <<error_matrix_accel<<endl;
    cout<<"show the inverse of accel's error matrix"<<endl
        <<error_matrix_accel.inverse()<<endl;
    auto error_matrix_accel_inv = error_matrix_accel.inverse();
    Eigen::Vector3d Bias = Eigen::Vector3d::Ones()*2.5;

    for(auto & info:accel_in_vector)
    {
        info = error_matrix_accel_inv * (info ) + Bias;
    }
    


    // std::cout<<"the transformed gyro data"<<endl;
    // showVector(gyro_in_vector,10);

    // ==================================== 间隔提取测试 =====================================
    // bool motion_interval = false;
    // int pre_index = 0;
    // std::vector<Eigen::Vector3d> gyro_in_motion_interval;

    // for(int i =0;i< gyro_real_in_vector.size();i++)
    // {
    //     if(!motion_interval)
    //     {
    //         if(gyro_real_in_vector.at(i).norm()!=0.0)
    //         {
    //             motion_interval = true;
    //             pre_index=i;
    //             gyro_in_motion_interval.push_back(gyro_in_vector.at(i));
    //         }
    //     }
    //     else
    //     {
    //         if(gyro_real_in_vector.at(i).norm()==0.0)
    //         {
    //             motion_interval = false;
    //             std::cout<<"the previous index is "<< pre_index<<std::endl;
    //             std::cout<<"the current index is "<<i<<std::endl;
    //             std::cout<<"the gyro_in_motion_interval size is "<< gyro_in_motion_interval.size()<<std::endl;
    //             showVector(gyro_in_motion_interval,10);
    //             gyro_in_motion_interval.clear();
    //             continue;
    //         }
    //         gyro_in_motion_interval.push_back(gyro_in_vector.at(i));
    //     }  
    // }    
    // return 0;

    // ================================= 

    // ================================= 优化测试 gyro ============================================

    CalibGyro calib_gyro(&gyro_in_vector,&gyro_real_in_vector,&gravity_in_vector,100.0);
    calib_gyro.buildProblem();

    std::cout<<"show the target error model is "<<endl
             << error_matrix<<endl;
    std::cout<<"show error matrix inverse"<<endl
             << error_matrix_inv<<endl;
    std::cout<<"show the estimation error matrix is "<<endl
             << calib_gyro.getErrorMatrix()<<endl;

    // ================================= 优化测试 accel ===========================================

    CalibAccel calib_accel(&accel_in_vector,&gyro_real_in_vector,9.7948419722650399);
    calib_accel.buildProblem();
    cout<<"show the accel's error matrix"<<endl
        <<error_matrix_accel<<endl;
    cout<<"the esitmated bias is "<<endl
        << calib_accel.getBias()<<endl;

    // for(auto & info:gravity_in_vector)
    // {
    //     cout<<"show the real accel"<< endl
    //         << (error_matrix_accel * info).transpose()<<endl;
    //     cout<<"show the transformed accel"<< endl
    //         << (calib_accel.getErrorMatrix() * info).transpose()<<endl;
    // }
}
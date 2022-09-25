/*
 * @Author: Liu Weilong
 * @Date: 2020-11-18 07:23:29
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2020-12-02 08:14:41
 * @Description:  因为IEKF 和 固定xk-1 估计 xk 的优化是等价的 所以这里直接使用 预积分+观测一起进行优化的形式在实现EKF
 */
#pragma once
#define TEST
#define TEST_PRINT
#define TEST_OPT
#include "ImuTypes.h"

#include "Eigen/Eigen"
#include "sophus/so3.hpp"
#include "ceres/ceres.h"

#include <iostream>
using namespace std;

class EKF
{
    public:
    EKF(Eigen::Matrix<double,6,6> laser_measurement_covar,
        IMU::Bias bias,
        IMU::Calib calib,
        Eigen::Matrix<double,15,1> initial_state = Eigen::Matrix<double,15,1>::Zero(), 
        Eigen::Matrix<double,15,15> initial_covar = Eigen::Matrix<double,15,15>::Zero()):
        imu_preintegrated_(bias,calib),
        cur_state_(Eigen::Matrix<double,15,1>::Zero()),
        cur_covar_(initial_covar),
        laser_meassurement_covar_(laser_measurement_covar),
        pre_state_(initial_state)
        {}

    // interface
    void addImuMeasurement(const Eigen::Vector3d &acceleration, const Eigen::Vector3d &angVel, const float &dt);
    void addLaserMeasurement(Eigen::Vector3d delta_rotation, Eigen::Vector3d delta_translation);
    
    auto getResult() const;
    Eigen::Vector3d getPosition() const;
    Eigen::Matrix<double,6,1> getBias() const;
    
   
    // internal
    void updateImuPreintegration(IMU::Bias bias,IMU::Calib calib);
    void buildProblem(Eigen::Vector3d rotation, Eigen::Vector3d translation);
    void Update();
    void UpdateCovar(ceres::Problem & problem);
    

    IMU::Preintegrated imu_preintegrated_;
    Eigen::Matrix<double,15,1>  pre_state_;
    Eigen::Matrix<double,15,1>  cur_state_;
    Eigen::Matrix<double,15,15> cur_covar_;
    Eigen::Matrix<double,6,6>   laser_meassurement_covar_;
};

// Mean Analysis
// X_k+1 = X_k + delta_X_m
// delta_X_m = [dR*exp(JRg*dbg),
//              dV+JVg*dbg+JVa*dba,
//              dP+JPg*dbg+JPa*dba,
//              0,
//              0]
// 
// delta_X_m = f(dX);
// ->
// delta_X = delta_X_m
// Covar Analysis
// covar(delta_X) = piror_covar + covar(delta_X_m)
class EKFPredictError:public ceres::SizedCostFunction<15,3,3,3,3,3>
{
    public:
    EKFPredictError(IMU::Preintegrated * IMU_preintegration,
                    Eigen::Matrix<double,15,15> & piror_covar,
                    Eigen::Matrix<double,15,1> & pre_state):
                    IMU_preintegration_(IMU_preintegration),
                    piror_covar_(piror_covar),
                    pre_state_(pre_state)
    {
        Eigen::Matrix<double,15,15> Covar = TypeTransform(IMU_preintegration_->C);
        Eigen::LLT<Eigen::Matrix<double,15,15>> LLTSolver(Covar.inverse());
        Lt_=LLTSolver.matrixL().transpose();
        // cout<<"the INFO of IMU Prediction is "<< endl
        // << Lt_<<endl;
    }

    bool Evaluate(const double *const * params,
                  double * residual ,
                  double ** jacobians) const 
    {
        Eigen::Map<const Eigen::Vector3d> R_map(params[0]);
        Eigen::Map<const Eigen::Vector3d> V_map(params[1]);
        Eigen::Map<const Eigen::Vector3d> P_map(params[2]);
        Eigen::Map<const Eigen::Vector3d> Bias_g_map(params[3]);
        Eigen::Map<const Eigen::Vector3d> Bias_a_map(params[4]);

        Eigen::Map<Eigen::Matrix<double,15,1>> residual_map(residual);
        
        // Bias 更新
        IMU::Bias new_bias(Bias_a_map(0),Bias_a_map(1),Bias_a_map(2),
                           Bias_g_map(0),Bias_g_map(1),Bias_g_map(2));
        new_bias = new_bias + IMU_preintegration_->GetOriginalBias();

        residual_map.block(9,0,3,1) = Bias_g_map;
        residual_map.block(12,0,3,1) = Bias_a_map;
        
        // R更新
        Eigen::Vector3d dR_so3_measure = TypeTransform( IMU::LogSO3(IMU_preintegration_->GetDeltaRotation(new_bias)));
        Eigen::Vector3d delta_so3 = (Sophus::SO3d::exp(R_map)* Sophus::SO3d::exp(-1*dR_so3_measure)).log();
        residual_map.block(0,0,3,1) = delta_so3;

        // V更新
        residual_map.block(3,0,3,1) = V_map - TypeTransform(IMU_preintegration_->GetDeltaVelocity(new_bias));
        
        // P更新
        
        Eigen::Vector3d velocity_w = pre_state_.block(3,0,3,1);
        Eigen::Vector3d so3_i_w = pre_state_.block(0,0,3,1);
        Sophus::SO3d rotatoin_w_i = Sophus::SO3d::exp(-1*so3_i_w);
        Eigen::Vector3d velocity_i = rotatoin_w_i * velocity_w;

        Eigen::Vector3d dP_whole =  TypeTransform(IMU_preintegration_->GetDeltaPosition(new_bias)) +
                                    velocity_i*IMU_preintegration_->dT;
        
        residual_map.block(6,0,3,1) = P_map -dP_whole;

#ifdef TEST_PRINT
        // cout<<"the residual is "<<endl;
        // cout<<residual_map.transpose()<<endl;
#endif 
        
        // 添加协方差
        residual_map = Lt_ * residual_map;

        if (!jacobians) return true;
        double* jacobian = jacobians[0];
        if (!jacobian) return true;
        
        Eigen::Matrix<double,15,15> jacobian_matrix = Eigen::Matrix<double,15,15>::Identity();
        
        // R 更新 R_residual = exp(R_map)*(exp(dR)*exp(JRg*dbg)).inverse()
        
        // auto so3_whole = (Sophus::SO3d::exp(R_map)*Sophus::SO3d::exp(-1*dR_so3_measure)).log();
        
        Eigen::Matrix3d Jr_inverse = TypeTransform(
                                    IMU::InverseRightJacobianSO3(
                                    delta_so3.x(),delta_so3.y(),delta_so3.z()
                                    ));

        jacobian_matrix.block(0,0,3,3) = Jr_inverse*Sophus::SO3d::exp(dR_so3_measure).matrix();
        
        //       对 dbg 求导
        
        jacobian_matrix.block(0,9,3,3) = -1*Jr_inverse*
                                            Sophus::SO3d::exp(dR_so3_measure).matrix()*
                                            TypeTransform(IMU_preintegration_->JRg);

        // PV 更新 P_residual  = P_var - (P_meas+ JPg * dbg_var + JPa * dba_var)
        //        V_residual = V_var - (V_meas + JVg * dbg_var + JVa * dba_var) 
        jacobian_matrix.block(3,9,3,3) = -1* TypeTransform(IMU_preintegration_->JVg);
        jacobian_matrix.block(3,12,3,3)= -1* TypeTransform(IMU_preintegration_->JVa);
        jacobian_matrix.block(6,9,3,3) = -1* TypeTransform(IMU_preintegration_->JPg);
        jacobian_matrix.block(6,12,3,3)= -1* TypeTransform(IMU_preintegration_->JPa);

        int count =0;
        for(int k =0;k<5;k++)
        {
            for(int i =0;i<15;i++)
            {
                for(int j = k*3;j<(k+1)*3;j++)
                {
                    jacobians[k][count] = jacobian_matrix(i,j);
                    count++;
                }
            }
            count = 0;
        }

        return true;
    }
 
    static ceres::CostFunction * Create(IMU::Preintegrated * IMU_preintegration,
                                        Eigen::Matrix<double,15,15> & piror_covar,
                                        Eigen::Matrix<double,15,1> & pre_state)
    {
        return new EKFPredictError(IMU_preintegration,piror_covar,pre_state);
    }

    private:
    IMU::Preintegrated * IMU_preintegration_;
    Eigen::Matrix<double,15,15> piror_covar_;
    Eigen::Matrix<double,15,1> pre_state_;
    Eigen::Matrix<double,15,15> Lt_;
};

// Measurement from lidar or gps
// Mean Analysis
// delta_rotation_m = delta_r
// delta_translation_m = delta_t
// Covar Analysis
// covar(delta_r) ~ covar(delta_rotation_m)
// covar(delta_t) ~ covar(delta_translation_m) 
class EKFObserError: public ceres::SizedCostFunction<6,3,3>
{
    public:
    EKFObserError(Eigen::Matrix<double,6,6> laser_measurement_covar,
                  Eigen::Vector3d delta_rotation_meas, Eigen::Vector3d delta_translation_meas):
                  laser_measurement_covar_(laser_measurement_covar),
                  delta_rotation_(delta_rotation_meas),
                  delta_translation_(delta_translation_meas)
    {
        Eigen::LLT<Eigen::Matrix<double,6,6>> LLTSolver(laser_measurement_covar_.inverse());
        Lt_=LLTSolver.matrixL().transpose();
        // cout<<"the Laser Measure INFO is "<<endl
        // <<Lt_<<endl;
    }
    
    bool Evaluate(const double *const * params,
                  double * residual ,
                  double ** jacobians) const 
    {
        Eigen::Map<const Eigen::Vector3d> rotation_map(params[0]);
        Eigen::Map<const Eigen::Vector3d> translation_map(params[1]);
        Eigen::Map<Eigen::Matrix<double,6,1>> residual_map(residual);
        
        Eigen::Vector3d delta_so3 = (Sophus::SO3d::exp(rotation_map)*Sophus::SO3d::exp(-1*delta_rotation_)).log();
        
        residual_map.block(0,0,3,1) = delta_so3;
        residual_map.block(3,0,3,1) = translation_map - delta_translation_;
        
        if (!jacobians) return true;
        double* jacobian = jacobians[0];
        if (!jacobian) return true;

        Eigen::Matrix<double,6,6> jacobian_matrix = Eigen::Matrix<double,6,6>::Identity();
        // Eigen::Vector3d so3_T = Sophus::SO3d::exp(-1*delta_rotation_).log();
        // auto so3_whole = (Sophus::SO3d::exp(rotation_map)*Sophus::SO3d::exp(so3_T)).log();
        Eigen::MatrixXd Jr_inverse = TypeTransform(
                              IMU::InverseRightJacobianSO3(
                              delta_so3.x(),delta_so3.y(),delta_so3.z()
                              ));

        jacobian_matrix.block(0,0,3,3) = Jr_inverse * Sophus::SO3d::exp(delta_rotation_).matrix();  

        int count =0;
        for(int k =0;k<2;k++)
        {
            for(int i =0;i<6;i++)
            {
                for(int j = k*3;j<(k+1)*3;j++)
                {
                    jacobians[k][count] = jacobian_matrix(i,j);
                    count++;
                }
            }
            count = 0;
        }

        return true;
    }

    static ceres::CostFunction * Create(Eigen::Matrix<double,6,6> laser_measurement_covar,
                                        Eigen::Vector3d delta_rotation_meas, 
                                        Eigen::Vector3d delta_translation_meas)
    {
        return new EKFObserError(laser_measurement_covar,delta_rotation_meas,delta_translation_meas);
    }

    private:
    Eigen::Matrix<double,6,6> laser_measurement_covar_;
    Eigen::Matrix<double,6,6> Lt_;
    Eigen::Vector3d delta_rotation_;
    Eigen::Vector3d delta_translation_;
};

class SO3LocalParameterization: public ceres::LocalParameterization
{
    public:
    virtual bool Plus(const double* x,
                const double* delta,
                double* x_plus_delta) const 
    {
        Eigen::Map<const Eigen::Vector3d> delta_x_map(delta);
        Eigen::Map<const Eigen::Vector3d> x_map(x);
        Eigen::Map<Eigen::Vector3d> new_so3(x_plus_delta);

        auto x_SO3 = Sophus::SO3d::exp(x_map);
        auto delta_x_SO3 = Sophus::SO3d::exp(delta_x_map);

        new_so3 = (x_SO3*delta_x_SO3).log();

        return true;
    }

    virtual bool ComputeJacobian(const double* x, double* jacobian) const
    {
        // 这里因为Plus 使用的是绕动模型 这里尽量保持一致
        static double eps = 1e-4;
        Eigen::Map<const Eigen::Vector3d> x_map(x);
        Eigen::Map<Eigen::Matrix<double,3,3>> jacobian_map(jacobian);
        if(x_map.norm()<eps)
        {
            jacobian_map = Eigen::Matrix3d::Identity();
            return true;
        }
        else
        {
            Eigen::Matrix3d W = Sophus::SO3d::hat(x_map);
            double d = x_map.norm();
            double d2 = d*d;
            Eigen::Matrix3d Jr = Eigen::Matrix3d::Identity()-W*(1.0-cos(d))/d2 + W*W*(d-sin(d))/(d2*d);
            jacobian_map = Jr.inverse();
            return true;
        }
    }

    virtual int GlobalSize() const { return 3; }
    virtual int LocalSize() const { return 3; }
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
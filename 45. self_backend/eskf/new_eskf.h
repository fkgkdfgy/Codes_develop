/*
 * @Author: Liu Weilong
 * @Date: 2021-05-22 21:24:56
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-05-30 22:27:35
 * @Description: 
 * 验证算法思想
 * 
 * ESKF 仅为IMU + GPS 版本
 * 
 * 
 * 2021.5.30  现在仅仅测试 SO3 上的更新
 * 
 */
#pragma once

#include <iostream>

#include "data_loader.h"
#include "sophus/so3.hpp"
#include "utils/sophus_utils.h"

using namespace Eigen;
using Input = DataLoader::ElementInfo;



class ESKF
{
    public:

    class IMUNoiseModel
    {
        public:

        const bool IsInit()const {
            if(init) return true;
            return false;
        }

        bool CopyFrom(const IMUNoiseModel & imu_model)
        {
            if(!imu_model.IsInit())return false;
            this -> noise_acc = imu_model.noise_acc;
            this -> noise_gyro = imu_model.noise_gyro;
            this -> walk_noise_acc = imu_model.walk_noise_acc;
            this -> walk_noise_gyro = imu_model.walk_noise_gyro;
            this -> init = true;
            return true;
        }

        bool init = false;
        Eigen::Matrix3d noise_acc;
        Eigen::Matrix3d noise_gyro;
        Eigen::Matrix3d walk_noise_acc;
        Eigen::Matrix3d walk_noise_gyro;
    };
    class State
    {
        public:
        State(){
            posi.setZero();
            vel.setZero();
            theta.setZero();
            bias_acc.setZero();
            bias_gyro.setZero();
        }

        Vector3d posi;
        Vector3d vel;
        
        Vector3d theta;

        Vector3d bias_acc;
        Vector3d bias_gyro;
    };
    class StateCovariance
    {
        public:
        StateCovariance()
        {
            theta_cov.setZero();
        }
        Matrix3d theta_cov;
    };
    class Observation
    {
        public:
        Vector3d obs_posi;
        Vector3d obs_vel;
        Vector3d obs_theta;

        Matrix3d obs_cov_posi;
        Matrix3d obs_cov_vel;
        Matrix3d obs_cov_theta;

    };
    ESKF():ref_state_(),state_(),error_state_(){ref_vel_explorate_posi_.setZero();}

    bool Init(const State & init_state, const IMUNoiseModel & imu_model)
    {
        ref_state_ = init_state;
        state_ = init_state;
        if(imu_noise_model_.CopyFrom(imu_model))
        return true;
        return false;
    }

    void PushIMUInfo(const Input & input)
    {
        // 进行中值处理
        if(last_timestamp_ == 0.)
            last_input_ = input;
        
        Input mid_value_input;
        mid_value_input = MidValue(last_input_,input);

        PropagateRef(mid_value_input);
        PropagateMean(mid_value_input);
        last_timestamp_ =  input.timestamp;
        last_input_ = input;

        // 方差的传递
        PropagateRefCov(mid_value_input);
        PropagateCov(mid_value_input);

        last_ref_state_ = ref_state_;
        last_state_ = state_;
    }

    void PushObservation(const Observation & obs)
    {
        UpdateObservation(obs);
        RefreshState();
    }

    const State & GetRefState()const{return ref_state_;}
    const State & GetState()const{return state_;}

    private:

    // ! 还没有考虑初始化的问题
    void PropagateRef(const Input & input)
    {
        // propagate ref value
        double delta_timestamp = input.timestamp - last_timestamp_;
        Sophus::SO3d rotation_SO3 = Sophus::SO3d::exp(ref_state_.theta).matrix();
        Eigen::Matrix3d rotation = (rotation_SO3*Sophus::SO3d::exp(input.ref_gyro * delta_timestamp)).matrix();
        // Eigen::Matrix3d rotation = (rotation_SO3).matrix();
        

        ref_vel_explorate_posi_ += input.ref_vel*delta_timestamp;

        ref_state_.posi += delta_timestamp * ref_state_.vel 
                        +0.5*delta_timestamp* delta_timestamp *(rotation * input.ref_acc+g);

        ref_state_.vel += delta_timestamp *(rotation * input.ref_acc + g);

        ref_state_.theta = (rotation_SO3 * Sophus::SO3d::exp(input.ref_gyro * delta_timestamp)).log();
        
    }

    void PropagateMean(const Input & input)
    {
        // propagate mean value
        double delta_timestamp = input.timestamp - last_timestamp_;
        Sophus::SO3d rotation_SO3 = Sophus::SO3d::exp(state_.theta).matrix();
        Eigen::Matrix3d rotation = (rotation_SO3*Sophus::SO3d::exp(input.meas_gyro * delta_timestamp)).matrix();

        state_.posi += delta_timestamp * state_.vel 
                    +0.5*delta_timestamp* delta_timestamp *(rotation * input.meas_acc +g);

        state_.vel += delta_timestamp *(rotation * input.meas_acc +g);

        state_.theta = (Sophus::SO3d::exp(state_.theta) * Sophus::SO3d::exp(input.meas_gyro * delta_timestamp)).log();        
    }
    
    void PropagateErrorState(const Input & input)
    {
        // propagate error state cov
        double delta_timestamp = input.timestamp - last_timestamp_;
        Sophus::SO3d Omega = Sophus::SO3d::exp(input.meas_gyro*delta_timestamp);
        error_state_.theta = Omega.matrix().transpose()*error_state_.theta;
    }

    void PropagateRefCov(const Input & input)
    { 
    }

    void PropagateCov(const Input & input)
    {
        // propagate error state rotation cov
        double delta_timestamp = input.timestamp - last_timestamp_;
        Sophus::SO3d Omega = Sophus::SO3d::exp(input.meas_gyro*delta_timestamp);

        error_state_cov_.theta_cov = Omega.matrix().transpose()*
                                     error_state_cov_.theta_cov*
                                     Omega.matrix();

        Eigen::Matrix3d J_r = Eigen::Matrix3d::Zero();
        rightJacobianSO3(Omega.log(),J_r);
        error_state_cov_.theta_cov += J_r*
                                      imu_noise_model_.noise_gyro*
                                      J_r.transpose();
    }                   
    // void PropagateErrorState(const Input & input);
    
    // 得到K 和 P

    protected:

    void UpdateObservation(const Observation & obs)
    {
        // update rotation obs
        Vector3d error_state_obs = (Sophus::SO3d::exp(state_.theta).inverse()*
                                   Sophus::SO3d::exp(obs.obs_theta)).log();
        K_.setZero();
        K_ = error_state_cov_.theta_cov*(error_state_cov_.theta_cov+obs.obs_cov_theta).inverse();
        m_ = K_*(error_state_obs);
        P_k_k_ = (Eigen::Matrix3d::Identity()-K_)*error_state_cov_.theta_cov;
    }
    
    void RefreshState()
    {
        // SO3 角度 update 
        state_.theta = (Sophus::SO3d::exp(state_.theta)*Sophus::SO3d::exp(m_)).log();
        Eigen::Matrix3d J_r_inv;
        J_r_inv.setZero();
        rightJacobianInvSO3(m_,J_r_inv);
        error_state_cov_.theta_cov = J_r_inv*P_k_k_*J_r_inv.transpose();
    }

    Input MidValue(const Input & pre_input, const Input & cur_input)
    {
        Input result;
        result.meas_acc = 0.5*(pre_input.meas_acc+cur_input.meas_acc);
        result.meas_gyro = 0.5*(pre_input.meas_gyro+cur_input.meas_gyro);
        result.ref_acc = 0.5*(pre_input.ref_acc+cur_input.ref_acc);
        result.ref_gyro = 0.5*(pre_input.ref_gyro+cur_input.ref_gyro);
        result.ref_vel = 0.5*(pre_input.ref_vel+cur_input.ref_vel);
        result.atti = cur_input.atti;
        result.posi = cur_input.posi;
        result.timestamp = cur_input.timestamp;
        return result;
    }
    
    public:
    static Eigen::Vector3d g;
    
    Input last_input_;
    // test using
    Eigen::Vector3d ref_vel_explorate_posi_;

    State ref_state_;
    State last_ref_state_;
    State state_;
    State last_state_;
    State error_state_;
    IMUNoiseModel imu_noise_model_;
    StateCovariance error_state_cov_;
    
    // add for SO3 EKF test
    Eigen::Matrix3d K_;
    Eigen::Matrix3d P_k_k_;
    Eigen::Vector3d m_;
    
    double last_timestamp_;
};

Eigen::Vector3d ESKF::g = Eigen::Vector3d(0,0,9.7803253359);
/*
 * @Author: Liu Weilong
 * @Date: 2020-12-04 22:41:11
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2020-12-19 23:06:55
 * @Description: Kalman Filter interface
 */
#pragma once
#include <iostream>
#include <iomanip>
#include <vector>
#include "Eigen/Eigen"
#include "sophus/so3.hpp"
#include "sophus/geometry.hpp"

template <typename U, typename Z,int n>
class EKFInterface
{
    public:
    EKFInterface()=default;
    using State = Eigen::Matrix<double,n,1>;
    using Covar = Eigen::Matrix<double,n,n>;
    // interface
    virtual void Predict(const U & u) =0;
    virtual void Correct(const Z & z) =0;
    // get observability matrix
    virtual Eigen::MatrixXd GetOM() const =0;

    Covar GetCovar() const {return mCovar;}
    State GetCurState() const { return mState;}
    void SetCovar(const Covar & c){mCovar = c;}
    void SetState(const State & s){mState = s;}

    virtual ~EKFInterface(){}
    // internal 
    State mState = State::Zero();
    Covar mCovar = Covar::Identity()*-1.0;
};

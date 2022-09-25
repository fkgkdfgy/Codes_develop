/*
 * @Author: Liu Weilong
 * @Date: 2021-03-11 13:36:36
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-03-11 14:42:25
 * @FilePath: /3rd-test-learning/38. line_feature/sim/line_parameterization.h
 * @Description: 
 */

#pragma once


#include <eigen3/Eigen/Dense>
#include <ceres/ceres.h>

class LineOrthParameterization : public ceres::LocalParameterization
{
    virtual bool Plus(const double *x, const double *delta, double *x_plus_delta) const;
    virtual bool ComputeJacobian(const double *x, double *jacobian) const;
    virtual int GlobalSize() const { return 4; };
    virtual int LocalSize() const { return 4; };
};

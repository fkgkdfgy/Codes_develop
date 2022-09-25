/*
 * @Author: Liu Weilong
 * @Date: 2020-12-28 21:06:12
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-01-06 10:33:38
 * @Description: 
 */

#pragma once

#include "g2o/core/base_unary_edge.h"
#include "g2o/core/base_vertex.h"

#include <g2o/core/block_solver.h>
#include <g2o/core/optimization_algorithm_levenberg.h>
#include <g2o/core/optimization_algorithm_gauss_newton.h>
#include <g2o/core/optimization_algorithm_dogleg.h>
#include <g2o/solvers/dense/linear_solver_dense.h>


#include "Eigen/Eigen"
#include "sophus/so3.hpp"
using namespace std;

class SO3R3
{
    public:
    SO3R3()
    {
        _r3 = Eigen::Vector3d::Zero();
        _SO3 = Sophus::SO3d::exp(_r3); 
    }
    Eigen::Vector3d _r3;
    Sophus::SO3d _SO3;
};

class SO3R3Vertex :public g2o::BaseVertex<6,SO3R3>
{
    public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    virtual void setToOriginImpl() override{
        _estimate = SO3R3();
    }
    virtual void oplusImpl(const double * update) override
    {
        Eigen::Vector3d _r3_update(update+3);
        _estimate._r3 += _r3_update;
        Eigen::Vector3d _so3_update(update);
        _estimate._SO3 = Sophus::SO3d::exp(_so3_update)*_estimate._SO3;
    }
    virtual bool read(std::istream &in) {}
    virtual bool write(std::ostream &out) const {}
};

class EdgeICP :public g2o::BaseUnaryEdge<3,Eigen::Vector3d,SO3R3Vertex>
{
    public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    virtual void computeError() override
    {
        const SO3R3Vertex * vx = 
        static_cast<const SO3R3Vertex *>(_vertices[0]);
        const SO3R3 Twc = vx->estimate();
        Eigen::Vector3d & Xc = _measurement;
        _error = (Twc._SO3*Xw+Twc._r3)-Xc;
    }

    virtual void linearizeOplus() override 
    {
        const SO3R3Vertex *v = static_cast<const SO3R3Vertex *> (_vertices[0]);
        const SO3R3 Twc = v->estimate();
        static bool first = false;

        _jacobianOplusXi.setZero();
        // 用于检测_jacobianOplusXi 是不是提前初始化过
        // if(!first)
        // {
        // std::cout<<"check the jacobian init"<<_jacobianOplusXi.transpose()<<std::endl;
        // first = true;
        // }
        _jacobianOplusXi.block<3,3>(0,0) = -1*Sophus::SO3d::hat(Twc._SO3*Xw);
        _jacobianOplusXi.block<3,3>(0,3) = Eigen::Matrix3d::Identity();
    }

    virtual bool read(std::istream &in) {}

    virtual bool write(std::ostream &out) const {}
    
    Eigen::Vector3d Xw;
};
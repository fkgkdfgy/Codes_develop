/*
 * @Author: Liu Weilong
 * @Date: 2021-06-16 08:53:59
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-06-16 10:30:57
 * @Description: 
 */
#pragma once

#include "g2o/core/base_unary_edge.h"
#include "g2o/core/base_vertex.h"
#include "g2o/types/slam3d/vertex_se3.h"

#include <g2o/core/block_solver.h>
#include <g2o/core/optimization_algorithm_levenberg.h>
#include <g2o/core/optimization_algorithm_gauss_newton.h>
#include <g2o/core/optimization_algorithm_dogleg.h>
#include <g2o/solvers/dense/linear_solver_dense.h>

#include "Eigen/Eigen"
#include "sophus/so3.hpp"
#include "pcl/common/common.h"

using PointCloud = pcl::PointCloud<pcl::PointXYZI>;

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
    inline Eigen::Vector3d project(const Eigen::Vector3d & a)const
    {
        return _estimate._SO3.matrix()*a+_estimate._r3;
    }
    // virtual bool read(std::istream &in) {return true;}
    // virtual bool write(std::ostream &out) const {return true;}
};

class G2O_NDTInternalModule
{
    public:
        EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    
    
};

class EdgeP2D:public g2o::BaseUnaryEdge<1,Eigen::Vector3d,SO3R3Vertex>
{
    public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    class EdgeP2DInternalParam
    {
        public:
        EIGEN_MAKE_ALIGNED_OPERATOR_NEW

        Eigen::Vector3d mu,source_point;
        Eigen::Vector3d d;
        // 使用的时候，需要取逆
        Eigen::Matrix3d distribution;
        Eigen::Vector3d A;
        double C;

    };
    // 中心点
    EdgeP2D(const EdgeP2DInternalParam & param):internal_param_(param){}
    virtual void computeError() override
    {
        const SO3R3Vertex * vx = 
        static_cast<const SO3R3Vertex *>(_vertices[0]);
        const SO3R3 Twc = vx->estimate();
        const Eigen::Vector3d & Xc = internal_param_.source_point;
        const Eigen::Vector3d & d = internal_param_.d;
        Eigen::Vector3d A = vx->project(Xc) - internal_param_.mu;
        internal_param_.A = A;
        Eigen::Matrix3d dis_inv = Eigen::Matrix3d::Identity();
        internal_param_.distribution.ldlt().solveInPlace(dis_inv);
        internal_param_.C = -0.5*d(1)*A.transpose().dot(dis_inv*A);
        _error(0) = d(0) *exp(internal_param_.C) + d(2);

    }

    virtual void linearizeOplus() override 
    {
        const SO3R3Vertex *v = static_cast<const SO3R3Vertex *> (_vertices[0]);
        const SO3R3 Twc = v->estimate();
        static bool first = false;

        _jacobianOplusXi.setZero();

        

        _jacobianOplusXi.block<3,3>(0,0) = -1*Sophus::SO3d::hat(Twc._SO3*Xw);
        _jacobianOplusXi.block<3,3>(0,3) = Eigen::Matrix3d::Identity();
    }
      

    EdgeP2DInternalParam internal_param_;
};
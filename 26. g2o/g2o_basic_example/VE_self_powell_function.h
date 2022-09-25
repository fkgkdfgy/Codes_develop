/*
 * @Author: Liu Weilong
 * @Date: 2020-12-27 17:04:29
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-01-08 09:12:41
 * @FilePath: /3rd-test-learning/29. g2o/g2o_basic_example/VE_self_powell_function.h
 * @Description:  使用g2o 编写 powell function
 */

#include <iostream>
#include <cmath>


#include "Eigen/Eigen"

#include "g2o/core/base_unary_edge.h"
#include "g2o/core/base_binary_edge.h"
#include "g2o/core/base_multi_edge.h"
#include "g2o/core/base_vertex.h"

#include <g2o/core/block_solver.h>
#include <g2o/core/optimization_algorithm_levenberg.h>
#include <g2o/core/optimization_algorithm_gauss_newton.h>
#include <g2o/core/optimization_algorithm_dogleg.h>
#include <g2o/solvers/dense/linear_solver_dense.h>

class X :public g2o::BaseVertex<1,double>
{
    public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    virtual void setToOriginImpl() override{
        // _estimation 存储定点信息 类型和 Eigen::Vector3d 保持一致
        _estimate =0;
    }
    virtual void oplusImpl(const double * update) override
    {
        // 这里的更新值和 ceres 的 LocalParameterization 一致 都是 const double ×
        _estimate += update[0];
    }; 
    virtual bool read(std::istream &in) {}
    virtual bool write(std::ostream &out) const {}
};

class X4:public g2o::BaseVertex<4,Eigen::Vector4d>
{
    public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    virtual void setToOriginImpl() override{
        // _estimation 存储定点信息 类型和 Eigen::Vector3d 保持一致
        _estimate = Eigen::Vector4d::Zero();
    }
    virtual void oplusImpl(const double * update) override
    {
        // 这里的更新值和 ceres 的 LocalParameterization 一致 都是 const double ×
        Eigen::Map<const Eigen::Vector4d> update_map(update);
        _estimate += update_map;
    }; 
    virtual bool read(std::istream &in) {}
    virtual bool write(std::ostream &out) const {}
};

class F1:public g2o::BaseUnaryEdge<1,double, X4>
{
    public:
     EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    virtual void computeError() override
    {
        const X4 * vx = 
        static_cast<const X4 *>(_vertices[0]);
        const Eigen::Vector4d x4a = vx->estimate();

        _error(0, 0) = x4a(0)+10*x4a(1);
    }

    // virtual void linearizeOplus() override {
    //     const CurveFittingVertex *v = static_cast<const CurveFittingVertex *> (_vertices[0]);
    //     const Eigen::Vector3d abc = v->estimate();
        
    //     double y = exp(abc[0] * _x * _x + abc[1] * _x + abc[2]);
    //     _jacobianOplusXi[0] = -_x * _x * y;
    //     _jacobianOplusXi[1] = -_x * y;
    //     _jacobianOplusXi[2] = -y;
    // }

    virtual bool read(std::istream &in) {}

    virtual bool write(std::ostream &out) const {}

};

class F2:public g2o::BaseUnaryEdge<1,double,X4>
{
    public:
     EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    virtual void computeError() override
    {
        const X4 * vx = 
        static_cast<const X4 *>(_vertices[0]);
        const Eigen::Vector4d x4a = vx->estimate();

        _error(0, 0) = std::sqrt(5.0)*(x4a(2)-x4a(3));
    }

    // virtual void linearizeOplus() override {
    //     const CurveFittingVertex *v = static_cast<const CurveFittingVertex *> (_vertices[0]);
    //     const Eigen::Vector3d abc = v->estimate();
        
    //     double y = exp(abc[0] * _x * _x + abc[1] * _x + abc[2]);
    //     _jacobianOplusXi[0] = -_x * _x * y;
    //     _jacobianOplusXi[1] = -_x * y;
    //     _jacobianOplusXi[2] = -y;
    // }

    virtual bool read(std::istream &in) {}

    virtual bool write(std::ostream &out) const {}
};


class F3F4:public g2o::BaseUnaryEdge<2,Eigen::Vector2d,X4>
{
    public:
     EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    virtual void computeError() override
    {
        const X4 * vx = 
        static_cast<const X4 *>(_vertices[0]);
        const Eigen::Vector4d x4a = vx->estimate();

        _error(0, 0) = std::sqrt(10.0)*(x4a(0)-x4a(3))*(x4a(0)-x4a(3));
        _error(1,0)  = (x4a(1)-2.0*x4a(2))*(x4a(1)-2.0*x4a(2));
    }

    // virtual void linearizeOplus() override {
    //     const CurveFittingVertex *v = static_cast<const CurveFittingVertex *> (_vertices[0]);
    //     const Eigen::Vector3d abc = v->estimate();
        
    //     double y = exp(abc[0] * _x * _x + abc[1] * _x + abc[2]);
    //     _jacobianOplusXi[0] = -_x * _x * y;
    //     _jacobianOplusXi[1] = -_x * y;
    //     _jacobianOplusXi[2] = -y;
    // }

    std::vector<g2o::HyperGraph::Vertex *> & getVertices(){return _vertices;}
    
    virtual bool read(std::istream &in) {}

    virtual bool write(std::ostream &out) const {}
};
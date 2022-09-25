/*
 * @Author: Liu Weilong
 * @Date: 2020-12-24 14:07:52
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-03-13 20:28:02
 * @FilePath: /3rd-test-learning/29. g2o/g2o_basic_example/VE_self_curvefitting.h
 * @Description:  学习如何编写 g2o
 *                这个是给定 cosfunction 的编写
 *                powell function 则对应变化 costfunction 的编写
 * 
 */

#pragma once
#include <iostream>
#include <vector>

// g2o 顶点和边 的设置
#include "g2o/core/base_unary_edge.h"
#include "g2o/core/base_vertex.h"

// g2o 关于求解器的设置
#include <g2o/core/block_solver.h>
#include <g2o/core/optimization_algorithm_levenberg.h>
#include <g2o/core/optimization_algorithm_gauss_newton.h>
#include <g2o/core/optimization_algorithm_dogleg.h>
#include <g2o/solvers/dense/linear_solver_dense.h>

// g2o 关于 鲁棒核
#include <g2o/core/robust_kernel_impl.h>


#include "Eigen/Eigen"
#include "opencv2/core/core.hpp"


using namespace std;

// 曲线模型的定点：
// BaseVertex<3, Eigen::Vector3d> 3 猜测是和 update 有关
// Eigen::Vector3d 是_estimate 的类型
// 经过阅读源代码  int N,typename D N=3 这里主要是为了确定内部 Hessian阵的大小和 b 的行数
// typedef Eigen::Matrix<double,N,N> hessian
// typedef Eigen::Matrix<double,N,1> b
class CurveFittingVertex:public g2o::BaseVertex<3,Eigen::Vector3d>
{
    public:

    // g2o内部的存储估计是使用了 指针进行地存储，所以需要加一个 Eigen 字节对齐
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    virtual void setToOriginImpl() override{
        // _estimation 存储定点信息 类型和 Eigen::Vector3d 保持一致
        _estimate << 0,0,0;
    }
    
    virtual void oplusImpl(const double * update) override
    {
        // 这里的更新值和 ceres 的 LocalParameterization 一致 都是 const double ×
        Eigen::Map<const Eigen::Vector3d> update_map(update);
        _estimate+= update_map;
    } 

    // 这里的两个 read 和 write 并不知道 是干什么用的 暂且先写上
    virtual bool read(std::istream &in) {}

    // write 有一个 const  需要注意
    virtual bool write(std::ostream &out) const {}
};

// 误差模型
// BaseUnaryEdge<1,double,CurveFittingVertex>
// int N, typename T1, typename T2
// 1 代表误差维数
// 因为这里误差维数只有1 所以这个类型直接取 double 就可以了(最初的一版)
// double 这个是直接对应 BaseEdge 里面 _measurement 的类型 (最初的一版)
// 其实这里完全可以是 Vector2d 只是取决于如何使用_measuerment
// (更改之后) 换成 Vector2d 完全可以使用
// typedef Measurement T1
// Measurement _measurement;

// 最底层的BaseEdge 会使用 N 来定义  
// information_matrix 和 error
// typedef Eigen::Matrix<double,N,N> _information_matdix
// typedef Eigen::Matrix<double,N.1> _error
// BaseEdge
class CurveFittingEdge : public  g2o::BaseUnaryEdge<1,Eigen::Vector2d,CurveFittingVertex>
{
    public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    // CurveFittingEdge(double x):BaseUnaryEdge(),_x(x){}
    
    // 以下是最重要的两个重载 一个计算b 一个计算Jacobian
    // computeError 对应b  linearizeOplus 对jacobian
    // _vertices 应该是  BaseUnaryEdge 里 BaseVertex的指针
    // _error BaseUnaryEdge 的误差项
    // _measurement 是观测值
    virtual void computeError() override
    {
        double _x = _measurement(0);
        double y = _measurement(1);
        const CurveFittingVertex * v = 
        static_cast<const CurveFittingVertex *>(_vertices[0]);
        const Eigen::Vector3d abc = v->estimate();
        _error(0, 0) = y - std::exp(abc(0, 0) * _x * _x + abc(1, 0) * _x + abc(2, 0));
    }

    
    // _jacobianOplusXi 对应jaocbian 求导
    // 这个变量不属于BaseEdge 
    // 是在BaseUnary 这一层进行定义的

    // 注释掉 用于测试自动求导
    // virtual void linearizeOplus() override {
    //     const CurveFittingVertex *v = static_cast<const CurveFittingVertex *> (_vertices[0]);
    //     const Eigen::Vector3d abc = v->estimate();
        
    //     double _x = _measurement(0);
    //     double y = exp(abc[0] * _x * _x + abc[1] * _x + abc[2]);
    //     _jacobianOplusXi[0] = -_x * _x * y;
    //     _jacobianOplusXi[1] = -_x * y;
    //     _jacobianOplusXi[2] = -y;
    // }


    virtual bool read(std::istream &in) {}

    virtual bool write(std::ostream &out) const {}

    private: 
    // 本来应该是由两个观测
    // 一个 x 一个 y 然后建立起对应的关系 
    // 但是这里因为BaseUnary _measurement 只有一个 double 所以就没有办法了。
    // 把 y 放在_measurement x 单独放在这里
    // double _x ; 
};

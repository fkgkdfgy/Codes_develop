/*
 * @Author: Liu Weilong
 * @Date: 2021-03-13 19:56:09
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-03-13 21:19:25
 * @Description: 
 */
#pragma once 

#include <iostream>
#include <vector>
#include <fstream>

#include "Eigen/Eigen"
#include "sophus/so3.hpp"

#include "g2o/core/base_vertex.h"
#include "g2o/core/base_binary_edge.h"

using namespace Sophus;
using namespace Eigen;
using namespace std;


class PoseAndInstrinc
{
    public:
    PoseAndInstrinc(){} 
    explicit PoseAndInstrinc(double * data_addr){set_to(data_addr);}
    bool set_to(double * data_addr);
    bool init()const {return init_;}
    public:
    Sophus::SO3d rotation_;
    Eigen::Vector3d translation_;
    double focal_;
    double k1_,k2_;
    bool init_ = false;
};

bool PoseAndInstrinc::set_to(double * data_addr)
{
    rotation_ = SO3d::exp(Vector3d(data_addr[0], data_addr[1], data_addr[2]));
    translation_ = Vector3d(data_addr[3], data_addr[4], data_addr[5]);
    focal_ = data_addr[6];
    k1_ = data_addr[7];
    k2_ = data_addr[8];
    init_ = true;
}

class CameraVertex: public g2o::BaseVertex<9,PoseAndInstrinc>
{
    public:

    // g2o内部的存储估计是使用了 指针进行地存储，所以需要加一个 Eigen 字节对齐
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    virtual void setToOriginImpl() override{
        // _estimation 存储定点信息 类型和 Eigen::Vector3d 保持一致
        _estimate = PoseAndInstrinc();
    }
    
    virtual void oplusImpl(const double * update) override
    {        
        _estimate.rotation_ = SO3d::exp(Vector3d(update[0], update[1], update[2])) * _estimate.rotation_;
        _estimate.translation_ += Vector3d(update[3], update[4], update[5]);
        _estimate.focal_ += update[6];
        _estimate.k1_ += update[7];
        _estimate.k2_ += update[8];
    } 

    Vector2d project(const Vector3d &point) {
        Vector3d pc = _estimate.rotation_ * point + _estimate.translation_;
        pc = -pc / pc[2];
        double r2 = pc.squaredNorm();
        double distortion = 1.0 + r2 * (_estimate.k1_ + _estimate.k2_ * r2);
        return Vector2d(_estimate.focal_ * distortion * pc[0],
                        _estimate.focal_ * distortion * pc[1]);
    }


    // 这里的两个 read 和 write 并不知道 是干什么用的 暂且先写上
    virtual bool read(std::istream &in) {}

    // write 有一个 const  需要注意
    virtual bool write(std::ostream &out) const {}

};

class PointVertex : public g2o::BaseVertex<3, Vector3d> {
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW;

    PointVertex() {}

    virtual void setToOriginImpl() override {
        _estimate = Vector3d(0, 0, 0);
    }

    virtual void oplusImpl(const double *update) override {
        _estimate += Vector3d(update[0], update[1], update[2]);
    }

    virtual bool read(istream &in) {}

    virtual bool write(ostream &out) const {}
};

class EdgeProjection : public g2o::BaseBinaryEdge<2, Vector2d, CameraVertex, PointVertex> {
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW;

    virtual void computeError() override {
        auto v0 = (CameraVertex *) _vertices[0];
        auto v1 = (PointVertex *) _vertices[1];
        auto proj = v0->project(v1->estimate());
        _error = proj - _measurement;
    }
    // use numeric derivatives
    virtual bool read(istream &in) {}
    virtual bool write(ostream &out) const {}
};




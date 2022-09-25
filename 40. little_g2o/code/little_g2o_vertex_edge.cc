/*
 * @Author: Liu Weilong
 * @Date: 2021-05-02 15:53:20
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-05-02 21:54:20
 * @Description: 
 */
#include "little_g2o_vertex_edge.h"

namespace little_g2o
{

    BaseVertex::~BaseVertex()
    {
        if(data_ != nullptr )delete [] data_;
    }

    void PoseVertex::Init(const double * data) 
    {
        Eigen::Map<const Sophus::SE3d> se3d_map(data);
        Eigen::Map<Sophus::SE3d> se3(data_);
        se3 = se3d_map;
        init_ = true;
    }

    void PoseVertex::Update(const double *data )
    {
        Eigen::Map<const Sophus::SE3d> se3d_update_map(data);
        Eigen::Map<Sophus::SE3d> se3(data_);
        se3 = se3d_update_map*se3;
    }


    void PointVertex::Init(const double * data)
    {
        Eigen::Map<const Eigen::Vector3d> pt_map(data);
        Eigen::Map<Eigen::Vector3d> pt(data_);
        pt = pt_map;
        init_ = true;
    }

    void PointVertex::Update(const double * data)
    {
        Eigen::Map<const Eigen::Vector3d> pt_map(data);
        Eigen::Map<Eigen::Vector3d> pt(data_);
        pt += pt_map;
    }
};





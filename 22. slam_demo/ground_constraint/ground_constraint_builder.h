/*
 * @Author: Liu Weilong
 * @Date: 2021-01-18 13:59:22
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-01-18 14:41:47
 * @FilePath: /3rd-test-learning/22. slam_demo/ground_constraint/ground_constraint_builder.h
 * @Description: 
 */

#include "g2o/core/base_unary_edge.h"
#include "g2o/core/base_vertex.h"
#include "Eigen/Eigen"
#include "sophus/so3.hpp"
#include "opencv2/core/core.hpp"

#define POSI_IDX 3
#define ROT_IDX 0
using M61 = Eigen::Matrix<double,6,1>;


Eigen::Matrix3d InverseRightJacobianSO3(const Eigen::Vector3d & so3)
{
    Eigen::Matrix3d I = Eigen::Matrix3d::Identity();
    const double d2 = so3.norm()*so3.norm();
    const double d = so3.norm();

    Eigen::Matrix3d W = Sophus::SO3d::hat(so3);
    if(d<1e-4)
    {
        return I;
    }
    else
    {
        return I + W/2 + W*W*(1.0f/d2 - (1.0f+cos(d))/(2.0f*d*sin(d)));
    }
}



class SO3R3
{   
    public:

    //TODO  SO3d::exp(Eigen::Block) 需要测试一下
    SO3R3() {
        _r3 = Eigen::Vector3d::Zero();
        _SO3 = Sophus::SO3d::exp(_r3);
        _SO3inv = _SO3.inverse();
    }

    SO3R3(const M61 & so3R3)
    {
        _r3 = so3R3.block<3,1>(POSI_IDX,0);
        _SO3 = Sophus::SO3d::exp(so3R3.block<3,1>(ROT_IDX,0));
        _SO3inv = _SO3.inverse();
    }
    SO3R3& operator=(const SO3R3 & temp)
    {
        _r3 = temp._r3;
        _SO3 = temp._SO3;
        _SO3inv = _SO3.inverse();
        return *this;
    }

    SO3R3& operator=(const Eigen::Matrix4d & temp)
    {
        _r3 = temp.block<3,1>(0,3);
        Eigen::AngleAxisd aad(temp.block<3,3>(0,0));
        _SO3 = Sophus::SO3d::exp(aad.angle()*aad.axis());
        _SO3inv = _SO3.inverse();
        return *this;
    }

    Eigen::Vector3d _r3;
    Sophus::SO3d _SO3;
    Sophus::SO3d _SO3inv;
    Eigen::Vector3d map(Eigen::Vector3d Xw)const 
    {
        return (_SO3*Xw+_r3);
    }
    
    Eigen::Matrix4d matrix()const
    {
       Eigen::Matrix4d t = Eigen::Matrix4d::Zero();
       t.block<3,3>(0,0) = _SO3.matrix();
       t.block<3,1>(0,3) = _r3;
       return t;
    }

};

class SO3R3Vertex :public g2o::BaseVertex<6,SO3R3>
{
    public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    virtual void setToOriginImpl() override{
        _estimate._r3 = Eigen::Vector3d::Zero();
        _estimate._SO3 = Sophus::SO3d::exp(_estimate._r3);
    }
    
    virtual void oplusImpl(const double * update) override
    {
        // 采用左乘的模型
        Eigen::Map<const M61> update_temp(update);
        // std::cout<<"the update is "<< update_temp.transpose()<<std::endl;
        // std::cout<<"the before _r3 is "<< _estimate._r3.transpose()<<std::endl;
        _estimate._r3 += update_temp.block<3,1>(POSI_IDX,0);
        // std::cout<<"the update _r3 is "<< _estimate._r3.transpose()<<std::endl;
        _estimate._SO3 = Sophus::SO3d::exp(update_temp.block<3,1>(ROT_IDX,0))*_estimate._SO3;
    } 

    virtual bool read(std::istream &in) {return true;}

    virtual bool write(std::ostream &out) const {return true;}
};

class EdgeRPZSO3R3 :public g2o::BaseUnaryEdge<3,Eigen::Vector3d,SO3R3Vertex>
{
    public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    EdgeRPZSO3R3(){}

    bool read(std::istream& is){ return true;}

    bool write(std::ostream& os) const{return true;}

    virtual void computeError() override {
        
        const SO3R3Vertex* v1 = static_cast<const SO3R3Vertex*>(_vertices[0]);
        Eigen::Vector3d so3 = v1->estimate()._SO3.log();
        const SO3R3 T_camera_in_baselink = GetExtParams();
        Sophus::SO3d R_baselink_in_world = (T_camera_in_baselink._SO3*v1->estimate()._SO3).inverse();
        Eigen::Vector3d _negative_R_t_t = v1->estimate()._SO3inv*v1->estimate()._r3*(-1.0);

        _error(0,0) = _measurement.x() - _negative_R_t_t.z();
        _error(0,1) = 0 - R_baselink_in_world.log().x();
        _error(0,2) = 0 - R_baselink_in_world.log().y();
    }

    virtual void linearizeOplus() override;
    static const SO3R3 & GetExtParams();
};

const SO3R3 & EdgeRPZSO3R3::GetExtParams()
{
    static bool read__ = false;
    static SO3R3 so3r3__;
    if(!read__)
    {
        cv::Mat Tcw__;
        cv::FileStorage fin("/home/sunny/workspace_private/3rd-test-learning/22. slam_demo/ground_constraint/config/stereoIPC.yaml",CV_STORAGE_READ);
        fin["Tracking.TcwInit"]>>Tcw__;
        std::cout<<"the Tcw__ is "<<std::endl<<Tcw__<<std::endl;

        Eigen::Matrix4d transform;
        std::memcpy(transform.data(),Tcw__.data,Tcw__.cols*Tcw__.rows*8);

        std::cout<<"the Tcw eigen is "<<transform<<std::endl;
        Eigen::Matrix4d temp=transform.transpose();
        so3r3__ = temp;
        read__ = true;
    }

    return so3r3__;
}



void EdgeRPZSO3R3::linearizeOplus()
{
    _jacobianOplusXi.setZero();
    Eigen::Vector3d e3(0.0,0.0,1.0);
    Eigen::Vector3d e1(1.0,0.0,0.0);
    Eigen::Vector3d e2(0.0,1.0,0.0);
    
    const SO3R3Vertex* v1 = static_cast<const SO3R3Vertex*>(_vertices[0]);

    auto e3_R_t = e3.transpose()*v1->estimate()._SO3inv.matrix();
    
    _jacobianOplusXi.block<1,3>(0,3) = e3_R_t;
    _jacobianOplusXi.block<1,3>(0,0) = e3_R_t*Sophus::SO3d::hat(v1->estimate()._r3);

    const SO3R3 T_camera_in_baselink = GetExtParams();
    
    Sophus::SO3d R_baselink_in_world = (T_camera_in_baselink._SO3*v1->estimate()._SO3).inverse();
    Eigen::Matrix3d J33_RR;
    
    J33_RR.setZero(); 
    J33_RR = -1.0*InverseRightJacobianSO3(R_baselink_in_world.log())*T_camera_in_baselink._SO3.matrix();

    _jacobianOplusXi.block<2,3>(1,0) = J33_RR.block<2,3>(0,0);
}




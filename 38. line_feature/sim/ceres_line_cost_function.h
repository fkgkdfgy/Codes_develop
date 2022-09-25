/*
 * @Author: Liu Weilong
 * @Date: 2021-03-10 10:39:21
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-03-15 11:16:26
 * @FilePath: /3rd-test-learning/38. line_feature/sim/ceres_line_cost_function.h
 * @Description: 
 * 
 * 1. TPCostFunction 5 次 收敛   这个的编程纯在问题，对于问题理解存在问题
 * 2. Plucker 8 次 收敛 
 * 3. PL-SLAM 验证 收敛的结果有问题
 * 
 * 
 */
#pragma once

#include <iostream>
#include <vector>
#include <cmath>

#include "ceres/ceres.h"
#include "Eigen/Eigen"
#include "sophus/se3.hpp"
#include "sim_env.h"
#include "line_geometry.h"
#include "line_parameterization.h"

// 几何误差

//! 两点优化 Two Point CostFunction 线段优化
//! 这个函数存在问题已经废止
class TPCostFunction:public ceres::SizedCostFunction<4,6>
{
    public:
    TPCostFunction(Eigen::Vector2d start,Eigen::Vector2d end, const Camera * camera):
    start_in_regular(start),end_in_regular(end),cm(camera){}

    virtual bool Evaluate(double const * const * params,
                          double * residuals,
                          double **jacobians)const
    {
        Eigen::Map<const Eigen::Vector3d> p_start(params[0]);
        Eigen::Map<const Eigen::Vector3d> p_end(params[0]+3);
        
        Eigen::Vector3d start_in_camera = cm->w2c(p_start);
        Eigen::Vector3d end_in_camera = cm->w2c(p_end);
        
        Eigen::Vector2d start_obs = cm->c2p(cm->w2c(p_start));
        Eigen::Vector2d end_obs = cm->c2p(cm->w2c(p_end));

        Eigen::Map<Eigen::Vector2d> r_start(residuals);
        Eigen::Map<Eigen::Vector2d> r_end(residuals+2);
        
        r_start = start_obs - start_in_regular;
        r_end = end_obs - end_in_regular;

        if (!jacobians) return true;
        double* jacobian = jacobians[0];
        if (!jacobian) return true;

        Eigen::Map<Eigen::Matrix<double,4,6,Eigen::RowMajor>> jaco(jacobians[0]);
        jaco.setZero();
        
        jaco.block<2,3>(0,0) = cm->J_uv_xyz(start_in_camera)*cm->T_w_c.inverse().so3().matrix();
        jaco.block<2,3>(2,3) = cm->J_uv_xyz(end_in_camera)*cm->T_w_c.inverse().so3().matrix();

        return true;
    }
    
    private:
    Eigen::Vector2d start_in_regular;
    Eigen::Vector2d end_in_regular;
    const Camera * cm;
};

class TPCostFunctionNew: public ceres::SizedCostFunction<2,6>
{

    public:
    TPCostFunctionNew(Eigen::Vector3d line_direction, const Camera * camera):
        ld(line_direction),cm(camera){}

    virtual bool Evaluate(double const * const * params,
                          double * residuals,
                          double **jacobians)const
    {
        Eigen::Map<const Eigen::Vector3d> p_start(params[0]);
        Eigen::Map<const Eigen::Vector3d> p_end(params[0]+3);
        
        Eigen::Vector3d start_in_camera = cm->w2c(p_start);
        Eigen::Vector3d end_in_camera = cm->w2c(p_end);
        
        Eigen::Vector2d start = cm->c2p(cm->w2c(p_start));
        Eigen::Vector2d end = cm->c2p(cm->w2c(p_end));
    
        Eigen::Map<Eigen::Vector2d> error(residuals);
        
        error.x() = ld.dot((Eigen::Vector3d()<<start,1.0).finished());
        error.y() = ld.dot((Eigen::Vector3d()<<end,1.0).finished());
        
        cout<<"the error : "<<error.transpose()<<endl;

        if (!jacobians) return true;
        double* jacobian = jacobians[0];
        if (!jacobian) return true;

        Eigen::Map<Eigen::Matrix<double,2,6,Eigen::RowMajor>> jaco(jacobians[0]);
        jaco.setZero();
        Eigen::Matrix3d J_uv_xyz_s,J_uv_xyz_e;
        J_uv_xyz_s.setZero();
        J_uv_xyz_e.setZero();
        J_uv_xyz_s.block<2,3>(0,0) = cm->J_uv_xyz(start_in_camera);
        J_uv_xyz_e.block<2,3>(0,0) = cm->J_uv_xyz(end_in_camera);

        jaco.block<1,3>(0,0) = ld.transpose()*J_uv_xyz_s*cm->T_w_c.inverse().so3().matrix();
        jaco.block<1,3>(1,3) = ld.transpose()*J_uv_xyz_e*cm->T_w_c.inverse().so3().matrix();

        return true;
    }
    
    private:
    const Camera * cm;
    Eigen::Vector3d ld;
};










// Dof4Line 无限长度的线
class Dof4LineCostFunction:public ceres::SizedCostFunction<2,6>
{
    public:
    Dof4LineCostFunction(Eigen::Vector3d start,Eigen::Vector3d end, const Camera * camera):
    start_in_regular(start),end_in_regular(end),cm(camera){}
    virtual bool Evaluate(double const * const * params,
                          double * residuals,
                          double **jacobians) const
    {
        Eigen::Map<const Eigen::Matrix<double,6,1>> L_w(params[0]);
        Eigen::Matrix<double,6,1> L_c = cm->w2cl(L_w);
        Eigen::Vector3d nc = L_c.block<3,1>(0,0);

        Eigen::Map<Eigen::Vector2d> r(residuals);

        double l_norm = nc(0) * nc(0) + nc(1) * nc(1) ;
        double l_sqrtnorm = sqrt( l_norm );
        double l_trinorm = l_norm * l_sqrtnorm;

        double e1 = start_in_regular.x() * nc(0) + start_in_regular.y() * nc(1) + nc(2);
        double e2 = end_in_regular.x() * nc(0) + end_in_regular.y() * nc(1) + nc(2);

        r.x() = e1/l_sqrtnorm;
        r.y() = e2/l_sqrtnorm;

        if (!jacobians) return true;
        double* jacobian = jacobians[0];
        if (!jacobian) return true;

        Eigen::Map<Eigen::Matrix<double,2,6,Eigen::RowMajor>> jaco_e_Lc(jacobians[0]);
        
        Eigen::Matrix<double, 2, 3> jaco_e_l(2, 3);
        jaco_e_l << (start_in_regular.x()/l_sqrtnorm - nc(0) * e1 / l_trinorm ), (start_in_regular.y()/l_sqrtnorm - nc(1) * e1 / l_trinorm ), 1.0/l_sqrtnorm,
                (end_in_regular.x()/l_sqrtnorm - nc(0) * e2 / l_trinorm ), (end_in_regular.y()/l_sqrtnorm - nc(1) * e2 / l_trinorm ), 1.0/l_sqrtnorm;

        Eigen::Matrix<double, 3, 6> jaco_l_Lc(3, 6);
        jaco_l_Lc.setZero();
        jaco_l_Lc.block(0,0,3,3) = Eigen::Matrix3d::Identity();

        jaco_e_Lc = jaco_e_l * jaco_l_Lc * cm->J_lc_lw();
        
        return true;
    }

    Eigen::Vector3d start_in_regular;
    Eigen::Vector3d end_in_regular;
    const Camera * cm;
};

class Dof4LineLocalParameterization: public ceres::LocalParameterization
{
    public:
    Dof4LineLocalParameterization(const Camera * camera):cm(camera){}
    virtual bool Plus(const double* x,
                const double* delta,
                double* x_plus_delta) const 
    {
        Eigen::Map<const Eigen::Matrix<double,6,1>> L_w(x);
        Eigen::Map<Eigen::Matrix<double,6,1>> update_L_w(x_plus_delta);

        Eigen::Vector3d m = L_w.block<3,1>(0,0);
        Eigen::Vector3d l = L_w.block<3,1>(3,0);

        double n_m = m.norm();
        double n_l = l.norm();
        double norm = std::sqrt(n_m*n_m+n_l*n_l);
        double cosx = n_m/norm;
        double sinx = n_l/norm;
        double origin_phi = std::asin(sinx);


        Eigen::Matrix3d so3;
        so3<<m.normalized(),l.normalized(),m.normalized().cross(l.normalized());

        Eigen::Map<const Eigen::Vector3d> theta(delta);
        double phi = *(delta + 3);

        double update_phi = origin_phi + phi;
        //Vector3d theta = orth.head(3);
        //double phi = orth[3];
        double s1 = sin(theta[0]);
        double c1 = cos(theta[0]);
        double s2 = sin(theta[1]);
        double c2 = cos(theta[1]);
        double s3 = sin(theta[2]);
        double c3 = cos(theta[2]);
        Eigen::Matrix3d R;
        R <<
        c2 * c3,   s1 * s2 * c3 - c1 * s3,   c1 * s2 * c3 + s1 * s3,
                c2 * s3,   s1 * s2 * s3 + c1 * c3,   c1 * s2 * s3 - s1 * c3,
                -s2,                  s1 * c2,                  c1 * c2;

        Eigen::Matrix3d update_so3 = so3*R;

        Eigen::Matrix<double,3,2> ml_so3 = update_so3.block<3,2>(0,0)*Sophus::SO2d::exp(update_phi).matrix();

        update_L_w.block<3,1>(0,0) = ml_so3.col(0);
        update_L_w.block<3,1>(3,0) = ml_so3.col(1);

        return true;
    }

    virtual bool ComputeJacobian(const double* x, double* jacobian) const
    {
        Eigen::Map<const Eigen::Matrix<double,6,1>> L_w(x);
        
        Eigen::Map<Eigen::Matrix<double,6,4,Eigen::RowMajor>> jaco(jacobian);
        Eigen::Vector3d e1(1.0,0.0,0.0);
        Eigen::Vector3d e2(0.0,1.0,0.0);

        Eigen::Vector3d m = L_w.block<3,1>(0,0);
        Eigen::Vector3d l = L_w.block<3,1>(3,0);
        
        Eigen::Matrix3d so3;
        so3<<m.normalized(),l.normalized(),m.normalized().cross(l.normalized());

        // cout<<" check m "<< m.transpose()<<endl;
        // cout<<" check so3: "<<endl<<so3<<endl;
        
        double n_m = m.norm();
        double n_l = l.norm();
        double norm = std::sqrt(n_m*n_m+n_l*n_l);
        double cosx = n_m/norm;
        double sinx = n_l/norm;

        jaco.setZero();

        jaco.block<3,3>(0,0) = -1.*cosx * so3 * Sophus::SO3d::hat(e1);
        jaco.block<3,3>(3,0) = -1.*sinx * so3 * Sophus::SO3d::hat(e2);
        jaco.block<3,1>(0,3) = -1.*sinx * so3 * e1;
        jaco.block<3,1>(3,3) = cosx* so3 * e2;

        return true;

    }

    virtual int GlobalSize() const { return 6; }
    virtual int LocalSize() const { return 4; }


    const Camera * cm;
};


class lineProjectionFactor : public ceres::SizedCostFunction<2, 4>
{
  public:
    lineProjectionFactor(const Eigen::Vector4d &_obs_i,const Camera * camera);
    virtual bool Evaluate(double const *const *parameters, double *residuals, double **jacobians) const;
    void check(double **parameters);

    Eigen::Vector4d obs_i;
    Eigen::Matrix<double, 2, 3> tangent_base;
    const Camera * cm;
    
};

lineProjectionFactor::lineProjectionFactor(const Eigen::Vector4d &_obs_i,const Camera * camera) : obs_i(_obs_i),cm(camera)
{
};

/*
  parameters[0]:  Twi
  parameters[1]:  Tbc
  parameters[2]:  line_orth
*/
bool lineProjectionFactor::Evaluate(double const *const *parameters, double *residuals, double **jacobians) const

{
    Eigen::Vector4d line_orth( parameters[0][0],parameters[0][1],parameters[0][2],parameters[0][3]);
    Vector6d line_w = orth_to_plk(line_orth);

    Vector6d line_c = cm->w2cl(line_w);

    // 直线的投影矩阵K为单位阵
    Eigen::Vector3d nc = line_c.head(3);
    double l_norm = nc(0) * nc(0) + nc(1) * nc(1);
    double l_sqrtnorm = sqrt( l_norm );
    double l_trinorm = l_norm * l_sqrtnorm;

    double e1 = obs_i(0) * nc(0) + obs_i(1) * nc(1) + nc(2);
    double e2 = obs_i(2) * nc(0) + obs_i(3) * nc(1) + nc(2);
    Eigen::Map<Eigen::Vector2d> residual(residuals);
    residual(0) = e1/l_sqrtnorm;
    residual(1) = e2/l_sqrtnorm;


    //std::cout << residual <<"\n";
    if (jacobians)
    {

        Eigen::Matrix<double, 2, 3> jaco_e_l(2, 3);
        jaco_e_l << (obs_i(0)/l_sqrtnorm - nc(0) * e1 / l_trinorm ), (obs_i(1)/l_sqrtnorm - nc(1) * e1 / l_trinorm ), 1.0/l_sqrtnorm,
                (obs_i(2)/l_sqrtnorm - nc(0) * e2 / l_trinorm ), (obs_i(3)/l_sqrtnorm - nc(1) * e2 / l_trinorm ), 1.0/l_sqrtnorm;


        Eigen::Matrix<double, 3, 6> jaco_l_Lc(3, 6);
        jaco_l_Lc.setZero();
        jaco_l_Lc.block(0,0,3,3) = Eigen::Matrix3d::Identity();

        Eigen::Matrix<double, 2, 6> jaco_e_Lc;
        jaco_e_Lc = jaco_e_l * jaco_l_Lc;
        if (jacobians[0])
        {
            Eigen::Map<Eigen::Matrix<double, 2, 4, Eigen::RowMajor>> jacobian_lineOrth(jacobians[0]);
            Matrix6d invTwc = cm->J_lc_lw();

            //std::cout<<invTwc<<"\n";

            Vector3d nw = line_w.head(3);
            Vector3d vw = line_w.tail(3);
            Vector3d u1 = nw/nw.norm();
            Vector3d u2 = vw/vw.norm();
            Vector3d u3 = u1.cross(u2);
            Vector2d w( nw.norm(), vw.norm() );
            w = w/w.norm();

            Eigen::Matrix<double, 6, 4> jaco_Lw_orth;
            jaco_Lw_orth.setZero();
            jaco_Lw_orth.block(3,0,3,1) = w[1] * u3;
            jaco_Lw_orth.block(0,1,3,1) = -w[0] * u3;
            jaco_Lw_orth.block(0,2,3,1) = w(0) * u2;
            jaco_Lw_orth.block(3,2,3,1) = -w(1) * u1;
            jaco_Lw_orth.block(0,3,3,1) = -w(1) * u1;
            jaco_Lw_orth.block(3,3,3,1) = w(0) * u2;

            //std::cout<<jaco_Lw_orth<<"\n";

            jacobian_lineOrth = jaco_e_Lc * invTwc * jaco_Lw_orth;
        }

    }


    return true;
}










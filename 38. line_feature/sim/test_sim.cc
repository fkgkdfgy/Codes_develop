/*
 * @Author: Liu Weilong
 * @Date: 2021-03-10 09:28:03
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-05-22 22:50:01
 * @FilePath: /3rd-test-learning/38. line_feature/sim/test_sim.cc
 * @Description: 
 * 
 * 1. 纯旋转无法正确的生成plucker参数，必须有平移才能生成plucker
 * 
 * 
 * 
 */
#include "ceres_line_cost_function.h"
#include "line_geometry.h"

Camera cm;
Line l;
LinePx l_px;
Camera cm1,cm2,cm3,cm4;
Eigen::AngleAxisd delta_r(0.3,Eigen::Vector3d(0.,0.,1.));
Eigen::AngleAxisd delta_r1(0.3,Eigen::Vector3d(0.,1.,1.));
LinePx l_px1,l_px2,l_px3,l_px4;
cv::Mat img(480,752,CV_8U,255);
cv::Mat img1,img2,img3;

Vector4d CameraPixel2Pi(const Camera & cm,const LinePx & lx)
{
    Eigen::Vector3d o2 = cm.T_w_c.translation();
    Eigen::Vector3d p2s = cm.c2w(cm.p2c(lx.start_px));
    Eigen::Vector3d p2e = cm.c2w(cm.p2c(lx.end_px));
    return pi_from_ppp(o2,p2s,p2e);
}

void Init()
{
    cm.T_w_c = Sophus::SE3d::exp(Eigen::Matrix<double,6,1>::Zero());
    l_px.start_px.x() = 380;
    l_px.start_px.y() = 80;
    l_px.end_px.x() = 400;
    l_px.end_px.y() = 300;

    l.start_point = cm.p2c(l_px.start_px,3.0);
    l.end_point = cm.p2c(l_px.end_px,6.0);

    cm1.T_w_c.so3() = Sophus::SO3d::exp(delta_r.angle()*delta_r.axis())*cm.T_w_c.so3();
    cm1.T_w_c.translation() = Eigen::Vector3d(1.2,-0.8,0.3);
    cm2.T_w_c.so3() = Sophus::SO3d::exp(delta_r.angle()*delta_r.axis())*cm1.T_w_c.so3();
    cm2.T_w_c.translation() = Eigen::Vector3d(0.3,0.3,0.3);
    cm3.T_w_c.so3() = Sophus::SO3d::exp(delta_r1.angle()*delta_r1.axis())*cm1.T_w_c.so3();
    cm3.T_w_c.translation() = Eigen::Vector3d(-1.2,-0.8,0.);
    cm4.T_w_c.so3() = Sophus::SO3d::exp(delta_r1.angle()*delta_r1.axis())*cm2.T_w_c.so3();
    cm4.T_w_c.translation() = Eigen::Vector3d(-1.,-2.,0.);

    l_px1.start_px = cm1.c2p(cm1.w2c(l.start_point));
    l_px1.end_px = cm1.c2p(cm1.w2c(l.end_point));
    l_px2.start_px = cm2.c2p(cm2.w2c(l.start_point));
    l_px2.end_px = cm2.c2p(cm2.w2c(l.end_point));


    l_px3.start_px = cm3.c2p(cm3.w2c(l.start_point));
    l_px3.end_px = cm3.c2p(cm3.w2c(l.end_point));
    l_px4.start_px = cm4.c2p(cm4.w2c(l.start_point));
    l_px4.end_px = cm4.c2p(cm4.w2c(l.end_point));

    l_px1.ld = direction(l_px1.start_px,l_px1.end_px);
    l_px2.ld = direction(l_px2.start_px,l_px2.end_px);
    l_px3.ld = direction(l_px3.start_px,l_px3.end_px);
    l_px4.ld = direction(l_px4.start_px,l_px4.end_px);

    img1 = img.clone();
    img2 = img.clone();
    img3 = img.clone();

    l_px.Update();l_px1.Update();l_px2.Update();l_px3.Update();l_px4.Update();
}

void TestTP()
{
    // 
    Eigen::Matrix<double,6,1> tp;
    tp.block<3,1>(0,0) = l.start_point;
    tp.block<3,1>(3,0) = l.end_point;
    tp += Eigen::Matrix<double,6,1>::Ones()*0.5;
    cout<<"the tp : "<< tp.transpose()<<endl;
    cout<<"the l.start: "<<l.start_point.transpose()<<"  the l.end: "<<l.end_point.transpose()<<endl;

    // 开始建立优化
    ceres::Problem problem;
    problem.AddParameterBlock(tp.data(),6);

    ceres::CostFunction * t1 = new TPCostFunction(l_px1.start_px,l_px1.end_px,&cm1);
    ceres::CostFunction * t2 = new TPCostFunction(l_px2.start_px,l_px2.end_px,&cm2);
    ceres::CostFunction * t3 = new TPCostFunction(l_px3.start_px,l_px3.end_px,&cm3);
    ceres::CostFunction * t4 = new TPCostFunction(l_px4.start_px,l_px4.end_px,&cm4);
    problem.AddResidualBlock(t1,NULL,tp.data());
    problem.AddResidualBlock(t2,NULL,tp.data());
    problem.AddResidualBlock(t3,NULL,tp.data());
    problem.AddResidualBlock(t4,NULL,tp.data());

    ceres::Solver::Options options;
    ceres::Solver::Summary summary;
    options.linear_solver_type = ceres::DENSE_QR;
    options.minimizer_type = ceres::TRUST_REGION;
    options.trust_region_strategy_type = ceres::LEVENBERG_MARQUARDT;

    options.max_num_iterations = 8;
    options.function_tolerance = 1e-3;

    // 这个Solve 和那个不能用的solve 看起来还是有区分度的
    ceres::Solve(options,&problem,&summary);

    cout<<summary.BriefReport()<<endl;

    cout<<"the result is "<< tp.transpose()<<endl;
    
}

void TestTPnew()
{
    // 
    Eigen::Matrix<double,6,1> tp;
    tp.block<3,1>(0,0) = l.start_point;
    tp.block<3,1>(3,0) = l.end_point;
    tp += Eigen::Matrix<double,6,1>::Ones()*0.6;
    
    cout<<"the tp : "<< tp.transpose()<<endl;
    cout<<"the l.start: "<<l.start_point.transpose()<<"  the l.end: "<<l.end_point.transpose()<<endl;

    // 开始建立优化
    ceres::Problem problem;
    problem.AddParameterBlock(tp.data(),6);

    ceres::CostFunction * t1 = new TPCostFunctionNew(l_px1.ld,&cm1);
    ceres::CostFunction * t2 = new TPCostFunctionNew(l_px2.ld,&cm2);
    ceres::CostFunction * t3 = new TPCostFunctionNew(l_px3.ld,&cm3);
    ceres::CostFunction * t4 = new TPCostFunctionNew(l_px4.ld,&cm4);
    problem.AddResidualBlock(t1,NULL,tp.data());
    problem.AddResidualBlock(t2,NULL,tp.data());
    problem.AddResidualBlock(t3,NULL,tp.data());
    problem.AddResidualBlock(t4,NULL,tp.data());

    ceres::Solver::Options options;
    ceres::Solver::Summary summary;
    options.linear_solver_type = ceres::DENSE_QR;
    options.minimizer_type = ceres::TRUST_REGION;
    options.trust_region_strategy_type = ceres::LEVENBERG_MARQUARDT;

    options.max_num_iterations = 30;
    options.function_tolerance = 1e-3;

    // 这个Solve 和那个不能用的solve 看起来还是有区分度的
    ceres::Solve(options,&problem,&summary);

    cout<<summary.BriefReport()<<endl;

    cout<<"the result is "<< tp.transpose()<<endl;

    // plucker test
    Vector6d plucker_l;
    plucker_l.block<3,1>(3,0) = (l.start_point - l.end_point).normalized();
    Eigen::Vector3d l_w = plucker_l.block<3,1>(3,0);
    plucker_l.block<3,1>(0,0) = l.start_point.cross(l_w);
    cout<<"the origin plucker"<<plucker_l.transpose()<<endl;

    Vector6d plucker_est;
    plucker_est.block<3,1>(3,0) = (tp.block<3,1>(0,0) - tp.block<3,1>(3,0)).normalized();
    l_w = plucker_est.block<3,1>(3,0);
    plucker_est.block<3,1>(0,0) = tp.block<3,1>(0,0).cross(l_w);

    cout<<"the estimated plucker is "<< plucker_est.transpose()<<endl;

    
}

void TestPluckerSlef()
{
    // 测试Plucker生成
    Vector6d plucker_l;
    plucker_l.block<3,1>(3,0) = (l.start_point - l.end_point).normalized();
    Eigen::Vector3d l_w = plucker_l.block<3,1>(3,0);
    plucker_l.block<3,1>(0,0) = l.start_point.cross(l_w);
    cout<<plucker_l.transpose()<<endl;

    Vector6d plucker_est = plucker_l + Eigen::Matrix<double,6,1>::Ones()*0.1;
    // Vector6d plucker_est = plucker_l ;

    ceres::Problem problem; 
    problem.AddParameterBlock(plucker_est.data(),6,new Dof4LineLocalParameterization(&cm));
    ceres::CostFunction * f1 = new Dof4LineCostFunction(cm1.p2c(l_px1.start_px),cm1.p2c(l_px1.end_px),&cm1);
    ceres::CostFunction * f2 = new Dof4LineCostFunction(cm2.p2c(l_px2.start_px),cm2.p2c(l_px2.end_px),&cm2);
    ceres::CostFunction * f3 = new Dof4LineCostFunction(cm3.p2c(l_px3.start_px),cm3.p2c(l_px3.end_px),&cm3);
    ceres::CostFunction * f4 = new Dof4LineCostFunction(cm4.p2c(l_px4.start_px),cm4.p2c(l_px4.end_px),&cm4);
    
    problem.AddResidualBlock(f1,NULL,plucker_est.data());
    problem.AddResidualBlock(f2,NULL,plucker_est.data());
    problem.AddResidualBlock(f3,NULL,plucker_est.data());
    problem.AddResidualBlock(f4,NULL,plucker_est.data());

    ceres::Solver::Options options;
    ceres::Solver::Summary summary;
    options.linear_solver_type = ceres::DENSE_QR;
    options.trust_region_strategy_type = ceres::LEVENBERG_MARQUARDT;

    options.max_num_iterations = 50;
    // options.function_tolerance = 1e-3;
    // options.parameter_tolerance = 1e-4;

    ceres::Solve(options,&problem,&summary);

    cout<<summary.BriefReport()<<endl;

    cout<<"the result is "<< plucker_est.transpose()<<endl;
}

void TestPluckerPLVIO()
{
    // 测试Plucker生成
    Vector6d plucker_l;
    plucker_l.block<3,1>(3,0) = (l.start_point - l.end_point).normalized();
    Eigen::Vector3d l_w = plucker_l.block<3,1>(3,0);
    
    plucker_l.block<3,1>(0,0) = l.start_point.cross(l_w);

    Vector4d orth_l = plk_to_orth(plucker_l);
    
    cout<<orth_l.transpose()<<endl;

    Vector4d orth_esi = orth_l + Eigen::Matrix<double,4,1>::Ones()*0.5;
    // Vector6d plucker_est = plucker_l ;

    ceres::Problem problem; 
    problem.AddParameterBlock(orth_esi.data(),4,new LineOrthParameterization());
    Eigen::Vector4d obs1 = to_pl_vio(l_px1.start_px,l_px1.end_px,cm1);
    Eigen::Vector4d obs2 = to_pl_vio(l_px2.start_px,l_px2.end_px,cm2);
    Eigen::Vector4d obs3 = to_pl_vio(l_px3.start_px,l_px3.end_px,cm3);
    Eigen::Vector4d obs4 = to_pl_vio(l_px4.start_px,l_px4.end_px,cm4);

    ceres::CostFunction * f1 = new lineProjectionFactor(obs1,&cm1);
    ceres::CostFunction * f2 = new lineProjectionFactor(obs2,&cm2);
    ceres::CostFunction * f3 = new lineProjectionFactor(obs3,&cm3);
    ceres::CostFunction * f4 = new lineProjectionFactor(obs4,&cm4);
    
    problem.AddResidualBlock(f1,NULL,orth_esi.data());
    problem.AddResidualBlock(f2,NULL,orth_esi.data());
    problem.AddResidualBlock(f3,NULL,orth_esi.data());
    problem.AddResidualBlock(f4,NULL,orth_esi.data());

    ceres::Solver::Options options;
    ceres::Solver::Summary summary;
    options.linear_solver_type = ceres::DENSE_QR;
    options.trust_region_strategy_type = ceres::LEVENBERG_MARQUARDT;

    options.max_num_iterations = 50;
    // options.function_tolerance = 1e-3;
    // options.parameter_tolerance = 1e-4;

    ceres::Solve(options,&problem,&summary);

    cout<<summary.BriefReport()<<endl;

    cout<<"the result is "<< orth_esi.transpose()<<endl;
}

void TestPluckerGeneration()
{
    // 测试Plucker生成
    Vector6d plucker_l;
    plucker_l.block<3,1>(3,0) = (l.start_point - l.end_point).normalized();
    Eigen::Vector3d l_w = plucker_l.block<3,1>(3,0);
    plucker_l.block<3,1>(0,0) = l.start_point.cross(l_w);
    cout<<plucker_l.transpose()<<endl;

    // 测试生成
    Vector4d pi2 = CameraPixel2Pi(cm1,l_px1);
    Vector4d pi4 = CameraPixel2Pi(cm3,l_px3);

    Vector6d plucker_test = pipi_plk(pi2,pi4);
    double norm = plucker_test.block<3,1>(3,0).norm();
    plucker_test /= norm;

    cout<<plucker_test.transpose()<<endl;

    // 测试匹配
    Eigen::Vector3d ps = cm1.p2c(l_px1.start_px);
    Eigen::Vector3d pe = cm1.p2c(l_px1.end_px);

    Vector6d L_c = cm1.w2cl(plucker_l);
    cout<<ps.dot(L_c.block<3,1>(0,0))<<endl; // 基本是 0
    cout<<pe.dot(L_c.block<3,1>(0,0))<<endl; // 基本是 0

}

int main()
{
    Init();
    TestTPnew();


}
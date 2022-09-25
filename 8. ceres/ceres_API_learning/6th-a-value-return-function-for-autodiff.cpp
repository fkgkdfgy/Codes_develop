/*
 * @Author: Liu Weilong
 * @Date: 2020-09-22 07:23:32
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2020-10-18 16:46:30
 * @Description: 讨论ceres autodiff 的多种函数接口？？？
 *               实际讨论的是 如何在autodiff内部使用自定义的函数
 *               如果这个函数只是返回一个double 值而不是 T(也就是说无法处理 Jet) 应该怎么处理
 *               A function return a value 
 *               竟然真的可以  令人窒息的操作
 *               实际上是通过了计算把NumericalDiffCostFunction 计算出的数值转为了Jet 的类型来对 AutoDiff 进行适配
 *               根据ceres 的doc NumericalDiff 对于三方库会有更好的支持
 *
 *               重点注意：
 *               1. CostFunctor 对于Nonlinear的包裹方式
 */

#include "data.hpp"
#include <ceres/ceres.h>
#include <iostream>
#include <cmath>
#include <memory>
using namespace std;

struct NonlinearEquation
{
    NonlinearEquation(const double x,const double y):
    ober_x(x),ober_y(y){}
    bool operator()(const double * m,const double *c,double * residual)const
    {
        residual[0] = ober_y-exp(ober_x*m[0]+c[0]);
        return true;
    }
    const double ober_x;
    const double ober_y;
};

struct CostFunctor
{
    CostFunctor(const double x,const double y):
    ober_x(x),ober_y(y){
        
        // 通过 ceres_ndt 的测试，
        // 如果这里的包裹方式是包裹一个已经实例化的对象
        // 那么AutoDiffCostFunction 无法进行析构
        // 所以这里必须是new 一个新的对象出来

        nonliearEquation_.reset(new ceres::CostFunctionToFunctor<1,1,1>(
        new ceres::NumericDiffCostFunction<NonlinearEquation,ceres::CENTRAL,1,1,1>(
            new NonlinearEquation(ober_x,ober_y))));
    }

    template<typename T>
    bool operator()(const T * m,const T * c,T*residual) const
    {
        (*nonliearEquation_)(m,c,residual);
    }

    const double ober_x;
    const double ober_y;
    std::shared_ptr<ceres::CostFunctionToFunctor<1,1,1>> nonliearEquation_;
};


int main(int argc,char **argv)
{

    // 第二部分 构建求解的方程
    double m = 0.0;
    double c = 0.0;
    
    ceres::Problem problem;
    for(int i = 0;i<kNumObservations;i++)
    {
        ceres::CostFunction* cost_function =
            new ceres::AutoDiffCostFunction<CostFunctor, 1, 1, 1>(
                new CostFunctor(data[2 * i], data[2 * i + 1]));
        problem.AddResidualBlock(cost_function, NULL, &m, &c);
    }

    // 第三部分 配置求解器

    ceres::Solver::Options options;
    ceres::Solver::Summary summary;
    options.linear_solver_type = ceres::DENSE_QR;
    options.minimizer_progress_to_stdout = true;
    // options.max_num_iterations = 10;
    ceres::Solve(options,&problem,&summary);

    std::cout<<summary.BriefReport()<<std::endl;

    std::cout<<"initial_m is "<<0.0<<std::endl
    <<"initial_c is "<<0.0<<std::endl
    <<"m is "<<m<<std::endl<<"c is "<<c<<std::endl;


    return 0;
}
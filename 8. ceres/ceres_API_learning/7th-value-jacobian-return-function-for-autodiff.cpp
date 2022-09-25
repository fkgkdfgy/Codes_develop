/*
 * @Author: Liu Weilong
 * @Date: 2020-09-22 19:16:32
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2020-10-06 21:09:22
 * @FilePath: /3rd-test-learning/8. ceres/7th-value-jacobian-return-function-for-autodiff.cpp
 * @Description:  在尝试了令人窒息的6th 的之后，再来尝试 可以和Sophus 混合使用的7
 *                一个可以返回value 和 jacobian 的接口
 *                TODO 还没有编写完
 */

#include "data.hpp"
#include <ceres/ceres.h>
#include <iostream>
#include <cmath>
#include <memory>
using namespace std;

class NonlinearEquation:public ceres::SizedCostFunction<1,1,1>
{
    public:
    virtual bool Evaluate(const double * const * parameters, 
    double *residual, double ** jacobians) const
    {
        
        return true;
    }
}

struct CostFunctor
{
    CostFunctor(const double x,const double y):
    ober_x(x),ober_y(y){
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
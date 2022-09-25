/*
 * @Author: Liu Weilong
 * @Date: 2020-09-18 07:08:43
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2020-09-22 08:54:57
 * @Description: 如何编写一个在ceres中可用的函数
 *               目前分析： 之前写的函数不可用的原因是 没有进行jet部分的重载
 *               
 *               这里对应ceres 内 autodiff interface 使用template 的一版
 *               如果不使用ceres 定义好的jet 运算，就需要自己编写一个正确的计算Jet的函数
 *               具体见仿函数CostFunctor
 */

#include <iostream>
#include <ceres/ceres.h>
#include <cmath>
#include "data.hpp"

using namespace std;

// 第一步配置仿函数
struct CostFunctor
{
    CostFunctor(const double x,const double y ):ober_x(x),ober_y(y){}

    template<typename T>
    bool operator()(const T* const m,const T* const c,T* residual)const
    {
        Evaluate(m[0],c[0],residual[0]); 
        return true; 
    }

    //  提示：1. 这里的成员函数也需要是const，目前看起来在调用的时候，使用的是一个const functor 对象
    template<typename JetT>
    void Evaluate(const JetT m,const JetT c,JetT & residual) const
    {
         residual =ober_y -  ceres::exp(m*ober_x+c);
        cout<<"--------------------------------------------------"<<endl;
        cout<<"now we show the differece"<<endl;
        cout<<"from the ceres::exp,the real part is "<<residual.a<<endl<<
        "the dual part is"<<residual.v<<endl;

        // 之前求导公式一直写错
        Evaluate(m.a,c.a,residual.a);
        residual.v = -ober_x*std::exp(m.a*ober_x+c.a)*m.v -std::exp(m.a*ober_x+c.a)*c.v;
        cout<<"now we show the differece"<<endl;
        cout<<"from the self-define ,the real part is "<<residual.a<<endl<<
        "the dual part is"<<residual.v<<endl;
    }
    void Evaluate(const double  m,const double c,double& residual) const
    {
        residual = ober_y - std::exp(m*ober_x+c);
    }
    

    const double ober_x;
    const double ober_y;
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
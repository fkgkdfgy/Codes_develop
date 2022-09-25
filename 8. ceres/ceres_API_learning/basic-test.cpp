/*
 * @Author: Liu Weilong
 * @Date: 2020-09-12 21:01:53
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2020-09-13 22:00:16
 * @Description: ceres 初级使用案例   基础流程
 */

#include <iostream>
#include "ceres/ceres.h"

using namespace std;
using namespace ceres;

// 第一部分：构建一个代价函数，重载() 符号

struct CostFunctor{
    // 这里返回值一定要是bool 的原因是 在 AutoDiffCostFunction 内部调用了一个 bool Call(){return functor(...)} 
    // 所以这里的返回类型一定是bool
    // 编写注意事项：
    // 1. 这里的模板是一个函数模板而不是一个类模板
    // 2. operator() 需要进行const 标志
    // 3. operator() 需要返回bool
    template <typename T>
    bool operator()(const T * const x,T* residual)const{
        residual[0] = T(10.0) - x[0];
        return true;
    }
};

int main(int argc, const char** argv) {
    google::InitGoogleLogging(argv[0]);

    // 设置初值
    double initial_x = 5;
    double x = initial_x;

    // 第二部分：构建优化问题
    Problem problem;

    // <typename CostFunctor ,int residual, int N0>
    CostFunction * cost_function = 
        new AutoDiffCostFunction<CostFunctor,1,1>(new CostFunctor);

    // AddResidualBlocks cost function 是针对整个样本集，loss function  就只是cost function 里面的一个误差项
    // NULL 的位置就是用来放置鲁棒核函数的地方
    problem.AddResidualBlock(cost_function,NULL,&x);

    // 第三部分：配置求解器

    Solver::Options options;

    options.linear_solver_type = ceres::DENSE_QR;
    options.minimizer_progress_to_stdout = true;
    Solver::Summary summary;

    // 注意事项： Solve是在ceres下面 而不是 ceres::Solver 下面
    Solve(options, &problem,&summary);

    cout<<summary.BriefReport()<<endl;

    // 打印结果
    cout<<"the initial x is "<<initial_x<<endl;
    cout<<"the current x is "<<x<<endl;
    
    return 0;
}
/*
 * @Author: Liu Weilong
 * @Date: 2020-09-13 11:22:15
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2020-09-13 17:30:54
 * @Description: 第二阶段的ceres 测试 自己编写一下Powell's Function
 */


#include <iostream>
#include <math.h>
#include "ceres/ceres.h"


using namespace std;
using namespace ceres;
// 第一部分，编写fucntor 也即是 loss function

// 之前一直有问题的原因是 operator() 没有const 关键字
struct LossFunctor1{
    template <typename T>
    bool operator()(const T * const x1, const T* const x2, T* residual)const {
        residual[0] = x1[0] + 10.0*x2[0];
        return true;
    }
};

struct LossFunctor2{
    template <typename T>
    bool operator()(const T * const x3,const T * const x4,T* residual)const {
        residual[0] = std::sqrt(5.0)*(x3[0]-x4[0]);
        return true; 
    }
};

struct LossFunctor3{
    template <typename T>
    bool operator()(const T* const x2,const T*const x3,T* residual)const {
        residual[0]= (x2[0]-2.0*x3[0])*(x2[0]-2.0*x3[0]);
        return true;
    }
};

struct LossFunctor4{
    template <typename T>
    bool operator()(const T* const x1,const T* const x4, T*residual)const {
        residual[0]= std::sqrt(10.0)*(x1[0]- x4[0])*(x1[0]- x4[0]);
        return true;
    }
};


int main(int argc, const char** argv) {
    
    google::InitGoogleLogging(argv[0]);

    // 数值初始化
    double x1=3,x2=-1,x3=0,x4=1;

    // 第二部分，构建cost function 和构建问题;
    CostFunction * loss1 = new AutoDiffCostFunction<LossFunctor1,1,1,1>(
        new LossFunctor1
    );
    CostFunction * loss2 = new AutoDiffCostFunction<LossFunctor2,1,1,1>(
        new LossFunctor2
    );
    CostFunction * loss3 = new AutoDiffCostFunction<LossFunctor3,1,1,1>(
        new LossFunctor3
    );
    CostFunction * loss4 = new AutoDiffCostFunction<LossFunctor4,1,1,1>(
        new LossFunctor4 
    );
    
    Problem problem;
    problem.AddResidualBlock(loss1,NULL,&x1,&x2);
    problem.AddResidualBlock(loss2,NULL,&x3,&x4);
    problem.AddResidualBlock(loss3,NULL,&x2,&x3);
    problem.AddResidualBlock(loss4,NULL,&x1,&x4);

    // 第三部分：配置求解器

    Solver::Options options;
    options.linear_solver_type = DENSE_QR;
    options.minimizer_progress_to_stdout = true;
    Solver::Summary summary;
    
    ceres::Solve(options,&problem,&summary);

    std::cout << "Initial x1 = " << x1
            << ", x2 = " << x2
            << ", x3 = " << x3
            << ", x4 = " << x4
            << "\n";

    cout<<summary.BriefReport()<<endl;

    return 0;
}

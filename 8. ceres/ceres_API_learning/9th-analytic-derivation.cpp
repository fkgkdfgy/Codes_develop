/*
 * @Author: Liu Weilong
 * @Date: 2020-10-06 21:15:50
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2020-12-27 17:08:00
 * @Description: 使用 Analytic Diff 来进行代码编写，这个地方也就是补上了 7th的例子
 */

#include <iostream>
#include <ceres/ceres.h>

using namespace std;

// 这里Analytic Diff 的编写还是有一些细节可以看一下
// 1. Evaluate 必须有
// 2. Evaluate 有const 、return bool
class Rat43Analytic: public ceres::SizedCostFunction<1,4>
{
    public:
    Rat43Analytic(const double x,const double y):x_(x),y_(y){}
    virtual ~ Rat43Analytic(){}

    virtual bool Evaluate(double const * const * params,
                          double * residuals,
                          double **jacobians)const
    {
        const double b1 = params[0][0];
        const double b2 = params[0][1];
        const double b3 = params[0][2];
        const double b4 = params[0][3];

        const double t1 = exp(b2 -  b3 * x_);
        const double t2 = 1 + t1;
        const double t3 = pow(t2, -1.0 / b4);
        residuals[0] = b1 * t3 - y_;

        // jacobians 的判断用于适配 Evaluate 的两次调用
        if (!jacobians) return true;
        double* jacobian = jacobians[0];
        if (!jacobian) return true;

        const double t4 = pow(t2, -1.0 / b4 - 1);
        jacobian[0] = t3;
        jacobian[1] = -b1 * t1 * t4 / b4;
        jacobian[2] = -x_ * jacobian[1];
        jacobian[3] = b1 * log(t2) * t3 / (b4 * b4);
        return true;
    }
    private:
    double x_;
    double y_;
};


int main()
{

    // 这种 size 上的内容可以直接 使用 new 进行创建然后进行计算

    // double array[4] {0,0,0,0};
    // ceres::CostFunction * costFunc = new Rat43Analytic(10,20);
    // ceres::Problem problem;
    // problem.AddResidualBlock(costFunc,NULL,array);
    return 0;
}
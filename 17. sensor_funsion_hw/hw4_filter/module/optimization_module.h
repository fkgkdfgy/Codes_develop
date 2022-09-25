/*
 * @Author: Liu Weilong
 * @Date: 2020-12-21 07:37:13
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2020-12-22 07:16:44
 * @Description: 预积分 residual 模块 
 */
#include <iostream>
#include <iomanip>

#include "Eigen/Eigen"
#include "ceres/ceres.h"

#include "measure_module.h"

using namespace std;

bool RotationResidual(const double * RotMeas, double ** params, double * jacobian);
bool TranslationResidual(const double * TransMeas, double ** params, double * jacobian);

class LaserResidual
{
    public:
    LaserResidual(const Laser & lm, const LaserNoise * ln);

    bool AddRotationResidual(ceres::Problem & problem);
    bool AddTranslationResidual(ceres::Problem & problem);
    private:
    Laser mlm;
    LaserNoise mln;
};

class IMUResidual
{
    public:

    
};
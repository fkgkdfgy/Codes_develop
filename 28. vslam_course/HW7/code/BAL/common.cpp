/*
 * @Author: Liu Weilong
 * @Date: 2021-03-13 21:25:31
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-03-13 21:26:15
 * @Description: 
 */

#include "common.h"

void PerturbPoint3(const double sigma, double *point) {
    for (int i = 0; i < 3; ++i)
        point[i] += RandNormal() * sigma;
}

double Median(std::vector<double> *data) {
    int n = data->size();
    std::vector<double>::iterator mid_point = data->begin() + n / 2;
    std::nth_element(data->begin(), mid_point, data->end());
    return *mid_point;
}

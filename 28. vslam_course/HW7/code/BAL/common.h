/*
 * @Author: Liu Weilong
 * @Date: 2021-03-13 19:32:33
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-03-13 21:27:20
 * @Description: 
 */
#pragma once
#include <iostream>
#include <stdio.h>
#include "random.h"
#include "rotation.h"

// 一个非常好的提取函数
template<typename T>
void FscanfOrDie(FILE *fptr, const char *format, T *value) {
    int num_scanned = fscanf(fptr, format, value);
    if (num_scanned != 1)
        std::cerr << "Invalid UW data file. ";
}

void PerturbPoint3(const double sigma, double *point) ;
double Median(std::vector<double> *data) ;


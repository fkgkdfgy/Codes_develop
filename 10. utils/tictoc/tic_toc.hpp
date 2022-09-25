/*
 * @Author: Liu Weilong
 * @Date: 2020-09-23 16:19:04
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2020-12-21 13:20:25
 * @FilePath: /ORB_SLAM2/include/tic_toc.hpp
 * @Description: 函数当中进行使用的计时器
 */
#pragma once
#include <fstream>
#include <chrono>
#include <iostream>
#include <string>
#include <utility>

#define nanos(diff) std::chrono::duration_cast<std::chrono::nanoseconds>(diff)
#define micros(diff) std::chrono::duration_cast<std::chrono::microseconds>(diff)
#define millis(diff) std::chrono::duration_cast<std::chrono::milliseconds>(diff)


class TicToc
{
    public:
    using __tp = std::chrono::_V2::system_clock::time_point;
    using __tpp = std::pair<__tp,int64_t>;


    TicToc(const std::string funcName);
    TicToc(const std::string funcName, const std::string StatPath);

    void Tic();

    int64_t Toc();

    int64_t checkprint_nano();

    int64_t checkprint_micro();

    int64_t checkprint_milli();  
    ~TicToc();
    __tp start;
    bool average_want = false;
    bool save = false;
    int64_t average_sum = 0;
    unsigned int count = 0;
    const std::string funcName_;
    std::string StatPath_ = " ";    
    std::ofstream fout_;  
};
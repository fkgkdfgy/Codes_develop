/*
 * @Author: Liu Weilong
 * @Date: 2020-12-15 13:53:38
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2020-12-21 13:24:39
 * @FilePath: /ORB_SLAM2/src/tic_toc.cpp
 * @Description: 
 */
#include "tic_toc.hpp"

TicToc::TicToc(const std::string funcName):funcName_(funcName){
        start = std::chrono::high_resolution_clock::now();
    }
TicToc::TicToc(const std::string funcName,const std::string StatPath):
        funcName_(funcName),StatPath_(StatPath){
        fout_.open(StatPath_,std::ios_base::out);
        start = std::chrono::high_resolution_clock::now();
        save = true;
    }

void TicToc::Tic()
    {
        start = std::chrono::high_resolution_clock::now();
    }

int64_t TicToc::Toc()
{
    int64_t duration = checkprint_milli();
    if(average_want)
    {
        average_sum+=duration;
        count++;
    }
    if(save)
    {
        fout_<<duration<<std::endl;
    }
    return duration;
}

int64_t TicToc::checkprint_nano()
{
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff_1 = end - start;
    std::cout<<"=============== until now "<<funcName_<<" have used ============="
    <<std::endl<<" "<<
    nanos(diff_1).count()<<" nanoseconds"<<std::endl;
    return nanos(diff_1).count();
}
int64_t TicToc::checkprint_micro(){
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff_1 = end - start;
    std::cout<<"=============== until now "<<funcName_<<" have used ============="
    <<std::endl<<" "<<
    micros(diff_1).count()<<" microseconds"<<std::endl;
    return micros(diff_1).count();
}
int64_t TicToc::checkprint_milli()
{
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff_1 = end - start;
    std::cout<<"=============== until now "<<funcName_<<" have used ============="
    <<std::endl<<" "<<
    millis(diff_1).count()<<" milliseconds"<<std::endl;
    return millis(diff_1).count();
}
TicToc::~TicToc()
{
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;
    std::cout<<"=============== "<<funcName_<<" totally used ============="
    <<std::endl<<" "<<
    nanos(diff).count()<<" nanoseconds "<<" == "<<micros(diff).count()<<" macroseconds "<<
    " == "<<millis(diff).count()<<" milliseconds "<<std::endl;
    if(save)
    {
        fout_.close();
    }
}
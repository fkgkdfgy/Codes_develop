/*
 * @Author: Liu Weilong
 * @Date: 2020-12-04 23:14:23
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2020-12-22 07:30:17
 * @Description: 
 */
#pragma once
#include "Eigen/Eigen"
#include <iostream>


#define IMUPreIntegration

using namespace std;

class IMUNoise
{
    public:
    IMUNoise(){}
    IMUNoise(const IMUNoise & in)
    {
        mNg = in.mNg;
        mNa = in.mNa;
        mNgwalk = in.mNgwalk;
        mNawalk = in.mNawalk;
        UpdateCov();
    }

    IMUNoise & operator=(const IMUNoise & in)
    {
        mNg = in.mNg;
        mNa = in.mNa;
        mNgwalk = in.mNgwalk;
        mNawalk = in.mNawalk;
        UpdateCov();
        return *this;
    }

    bool UpdateCov()
    {
        if(mNg>0.0 && mNa>0.0 && mNgwalk>0.0 && mNawalk>0.0)
        {
            mCov = Eigen::Matrix<double,12,12>::Identity();
            mCov.block<3,3>(0,0) *= mNg;
            mCov.block<3,3>(3,3) *= mNa;
            mCov.block<3,3>(6,6) *= mNgwalk;
            mCov.block<3,3>(9,9) *= mNawalk;
            return true;
        }
        cout<<"[ERROR]: there must be something wrong with the Noise Setting!"<<endl;
        std::abort();
    }

    double mNg = -1.0;
    double mNa = -1.0;
    double mNgwalk= -1.0;
    double mNawalk= -1.0;
    Eigen::Matrix<double,12,12> mCov = Eigen::Matrix<double,12,12>::Identity() * -1.0;
};

class IMU 
{
    public:

#ifdef IMUPreIntegration   
    using F = Eigen::Matrix<double,15,15>;
    using BEuler = Eigen::Matrix<double,15,12>; 
    using BMidV = Eigen::Matrix<double,15,18>;
    using State = Eigen::Matrix<double,15,1>;
    bool GetFEuler(const State & state, F & oF);
    bool GetFMidV(const State & state, F & oF);
    bool GetBEuler(const State & state, BEuler & oB);
    bool GetBMidV(const State & state, BMidV & oB);
#endif
    Eigen::Vector3d mAccel;
    Eigen::Vector3d mGyro;
    double mTime = -1.0;
    // Eigen::Vector3d & GetAccBias()
    // {
    //     static Eigen::Vector3d AccBias = Eigen::Vector3d::Zero();
    //     return AccBias;    
    // }
    // Eigen::Vector3d & GetGyroBias()
    // {
    //     static Eigen::Vector3d GyroBias = Eigen::Vector3d::Zero();
    //     return GyroBias; 
    // }
};


class LaserNoise
{
    public:

    LaserNoise(){}
    LaserNoise(const LaserNoise & ln)
    {
        mRotN = ln.mRotN;
        mTraN = ln.mTraN;
        UpdateCov();
    }

    LaserNoise & operator= (const LaserNoise & ln)
    {
        mRotN = ln.mRotN;
        mTraN = ln.mTraN;
        UpdateCov();
        return *this;
    }

    bool UpdateCov()
    {
        if(mRotN>0.0 && mTraN>0.0)
        {
            mCov = Eigen::Matrix<double,6,6>::Identity();
            mCov.block<3,3>(0,0) *= mRotN;
            mCov.block<3,3>(3,3) *= mTraN;
            return true;
        }
        cout<<"[ERROR]: there must be something wrong with the Noise Setting!"<<endl;
        std::abort();
    }

    double mRotN= -1.0;
    double mTraN= -1.0;
    Eigen::Matrix<double,6,6> mCov = Eigen::Matrix<double,6,6>::Identity() * -1.0;
};
class Laser
{
    public:
    using LaserY = Eigen::Matrix<double,6,1>;
    using LaserG = Eigen::Matrix<double,6,15>;
    using LaserC = Eigen::Matrix<double,6,6>;
    using LaserK = Eigen::Matrix<double,15,6>;

    Eigen::Vector3d mDRot;
    Eigen::Vector3d mDTra;
    double mTime = -1.0;
};
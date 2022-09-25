/*
 * @Author: Liu Weilong
 * @Date: 2020-12-05 09:48:17
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-05-23 21:10:22
 * @Description:  ESKF 内部函数实现
 */

#include "error_state_kalman_filter.h"


Eigen::Vector3d gravity_vector_w(0,0,9.794841972265039942);

ESKF::ESKF(const Covar & c,const State & s,
           const IMUNoise & in, const LaserNoise & ln)
{
    mIN = in;   
    mLN = ln;
    mState = s;
    mCovar = c;
}

//TODO not check mIN’s mCov

void ESKF::Predict(const IMU & u)
{
    IMU ubIMU;
    DataProcess(u,ubIMU);

    // UpdateState UpdateErrorState
    MatrixG G;
    MatrixF F;
    UpdateState(ubIMU);
    UpdateErrorState(ubIMU,F,G);
    UpdateErrorStateCovarance(F,G);
    UpdateErrorStateToState();

}

void ESKF::DataProcess(const IMU & u, IMU & unIMU)
{
    unIMU.mAccel = u.mAccel - mState.block<3,1>(BA_IDX,0);
    unIMU.mGyro  = u.mGyro - mState.block<3,1>(BG_IDX,0);
    unIMU.mTime = u.mTime;
}

// TODO 单元测试
void ESKF::UpdateState(const IMU & ubIMU)
{
    double delta_t = 0.01;
    const Eigen::Vector3d & GyroNB = ubIMU.mGyro;
    const Eigen::Vector3d & AccNB = ubIMU.mAccel;
    Sophus::SO3d so3_k_n = Sophus::SO3d::exp(mState.block<3,1>(PHI_IDX,0));
    Sophus::SO3d so3_delta = Sophus::SO3d::exp(GyroNB*delta_t);
    
    mState.block<3,1>(PHI_IDX,0) = (so3_k_n*so3_delta).log();

    mState.block<3,1>(POS_IDX,0) += mState.block<3,1>(VEL_IDX,0)*delta_t +
                                    0.5*(so3_k_n*AccNB+gravity_vector_w)*delta_t*delta_t;
                                    
    mState.block<3,1>(VEL_IDX,0) += (so3_k_n*AccNB+gravity_vector_w)*delta_t;
}

// TODO 单元测试
void ESKF::UpdateErrorState(const IMU & ubIMU, MatrixF & F, MatrixG & G)
{
    F.setZero();
    G.setZero();
    
    // PHI 部分
    F.block<3,3>(PHI_IDX,PHI_IDX) = -1 * Sophus::SO3d::hat(ubIMU.mGyro);
    F.block<3,3>(PHI_IDX,BG_IDX) = -1 *I3;
    G.block<3,3>(PHI_IDX,NG_IDX) = I3;
    
    // VEL 部分
    Eigen::Vector3d so3 = mState.block<3,1>(PHI_IDX,0);
    Eigen::Matrix3d C_n_b = Sophus::SO3d::exp(so3).matrix();

    F.block<3,3>(VEL_IDX,BA_IDX) = -1*C_n_b;
    F.block<3,3>(VEL_IDX,PHI_IDX) = -1*C_n_b*Sophus::SO3d::hat(ubIMU.mAccel); 
    G.block<3,3>(VEL_IDX,NA_IDX) = C_n_b;

    // POS 部分
    F.block<3,3>(POS_IDX,VEL_IDX) = I3;
    
    // BA BG 部分
    G.block<3,3>(BA_IDX,NAW_IDX) = I3;
    G.block<3,3>(BG_IDX,NGW_IDX) = I3; 

#ifdef COV_CHECK
    std::cout<<"the F matrix is "<<std::endl
    <<std::setprecision(5)<<F<<std::endl;
    std::cout<<"the G matrix is "<<std::endl
    <<std::setprecision(5)<<G<<std::endl;
#endif // COV_CHECK

#ifdef F_1ST
    mErrorState = (I15 + F*0.01) *mErrorState;
#else
    mErrorState = (I15 + F*0.01 + 0.5*F*0.01*F*0.01)*mErrorState;
#endif
    
}

void ESKF::UpdateErrorStateCovarance(const MatrixF & F, const MatrixG & G)
{

#ifdef F_1ST
    mErrorCov = (I15 + F*0.01)*mErrorCov*(I15 + F*0.01).transpose();
#else
    mErrorCov = (I15 + F*0.01 + 0.5*F*0.01*F*0.01)*mErrorCov*(I15 + F*0.01 + 0.5*F*0.01*F*0.01).transpose();
#endif

    mErrorCov += (G*0.01)*mIN.mCov*(G*0.01).transpose();
}

void ESKF::Correct(const Laser & z)
{
    Laser::LaserY Y_l;
    Laser::LaserG G_l;
    Laser::LaserC C_l = Laser::LaserC::Identity();
    Laser::LaserK K_l;
    Y_l.setZero();
    G_l.setZero();
    BuildErrorStateObs(z.mDRot.data(),nullptr,z.mDTra.data(),
                       Y_l,G_l);
    
    K_l.setZero();
    K_l = mErrorCov*G_l.transpose()*
          ((G_l*mErrorCov*G_l.transpose()+C_l*mLN.mCov*C_l.transpose()).inverse());

    mErrorCov = (I15-K_l*G_l)*mErrorCov;

    mErrorState = mErrorState + K_l*(Y_l - G_l*mErrorState);
}

void ESKF::UpdateErrorStateToState()
{
    mState.block<12,1>(VEL_IDX,0) += mErrorState.block<12,1>(VEL_IDX,0);
    
    Sophus::SO3d SO3_old = Sophus::SO3d::exp(mState.block<3,1>(PHI_IDX,0));
    Sophus::SO3d SO3_delta = Sophus::SO3d::exp(mErrorState.block<3,1>(PHI_IDX,0));
    mState.block<3,1>(PHI_IDX,0) = (SO3_old*SO3_delta).log();

    mErrorState.setZero();
}

Eigen::MatrixXd ESKF::GetOM()const
{

}

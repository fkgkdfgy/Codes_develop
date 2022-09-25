/*
 * @Author: Liu Weilong
 * @Date: 2020-12-05 09:48:17
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2020-12-19 11:12:52
 * @Description:  EKF 内部函数实现
 */

#include "kalman_filter.h"

EKF::EKF(const Covar & c,const State & s,
           const IMUNoise & in, const LaserNoise & ln)
{
    mIN = in;   
    mLN = ln;
    mState = s;
    mCovar = c;
}

//TODO not check mIN’s mCov

void EKF::Predict(const IMU & u)
{
    // 均值更新
    static Eigen::Vector3d gravity_vector_w(0,0,9.794841972265039942);
    // Eigen::Matrix<double,15,15> F = Eigen::Matrix<double,15,15>::Zero();
    Eigen::Matrix<double,15,12> B = Eigen::Matrix<double,15,12>::Zero();
    double delta_t = 0.01;
    // PHI 相关

    Eigen::Vector3d AccNB = u.mAccel - mPreK.block<3,1>(BA_IDX,0);
    Eigen::Vector3d GyroNB = u.mGyro - mPreK.block<3,1>(BG_IDX,0);
    Sophus::SO3d phi_k_prior_SO3 = Sophus::SO3d::exp(mPreK.block<3,1>(PHI_IDX,0))* 
                                   Sophus::SO3d::exp(GyroNB*delta_t);
    Eigen::Vector3d phi_k_prior = (phi_k_prior_SO3).log();
    
    // PHI 的线性化
    // Eigen::Matrix3d Jp_p = Eigen::Matrix3d::Identity();
    // Jp_p = JrInv(phi_k_prior.x(),phi_k_prior.y(),phi_k_prior.z());
    // Jp_p = Jp_p * Sophus::SO3d::exp(-1*GyroNB*delta_t).matrix();

    // auto tmp_so3 = Sophus::SO3d::exp(GyroNB*delta_t).log();
    // Eigen::Matrix3d Jp_bg = Eigen::Matrix3d::Identity();
    // Jp_bg = JrInv(phi_k_prior.x(),phi_k_prior.y(),phi_k_prior.z());
    // Jp_bg = -1*Jp_bg*Jr(tmp_so3.x(),tmp_so3.y(),tmp_so3.z())*delta_t;

    // Eigen::Matrix3d Jp_nw = Eigen::Matrix3d::Identity();
    // Jp_nw = -1*Jp_bg;
    // V 相关

    Eigen::Vector3d v_k_piror = mPreK.block<3,1>(VEL_IDX,0);
    Eigen::Vector3d delta_v = (Sophus::SO3d::exp(mPreK.block<3,1>(PHI_IDX,0)).matrix()*
                               AccNB + gravity_vector_w)*delta_t;
    v_k_piror += delta_v;

    // V 线性化
    // Eigen::Matrix3d Jv_p,Jv_ba,Jv_na;
    // Jv_p = -1*Sophus::SO3d::exp(mPreK.block<3,1>(PHI_IDX,0)).matrix()
    //          *Sophus::SO3d::hat(AccNB*delta_t);
    // Jv_ba = -1*Sophus::SO3d::exp(mPreK.block<3,1>(PHI_IDX,0)).matrix()*delta_t;
    // Jv_na = Sophus::SO3d::exp(mPreK.block<3,1>(PHI_IDX,0)).matrix()*delta_t;

    // P 相关

    Eigen::Vector3d t_k_piror = mPreK.block<3,1>(POS_IDX,0);
    Eigen::Vector3d delta_p = mPreK.block<3,1>(VEL_IDX,0)*delta_t + 0.5*delta_v*delta_t;
    t_k_piror+=delta_p;

    // P线性化
    // Eigen::Matrix3d Jt_p,Jt_v,Jt_na,Jt_ba;
    // Jt_p = -0.5*Sophus::SO3d::exp(mPreK.block<3,1>(PHI_IDX,0)).matrix()
    //          *Sophus::SO3d::hat(AccNB)*delta_t*delta_t;
    // Jt_v = Eigen::Matrix3d::Identity()*delta_t;
    // Jt_ba = 0.5*Jv_ba*delta_t;
    // Jt_na = 0.5*Jv_na*delta_t;
    
    // F = Eigen::Matrix<double,15,15>::Identity();

    // F.block<3,3>(PHI_IDX,PHI_IDX) = Jp_p;
    // F.block<3,3>(PHI_IDX,BG_IDX) = Jp_bg;

    // F.block<3,3>(VEL_IDX,PHI_IDX) = Jv_p;
    // F.block<3,3>(VEL_IDX,BA_IDX) = Jv_ba;
    
    // F.block<3,3>(POS_IDX,POS_IDX) = Jt_p;
    // F.block<3,3>(POS_IDX,VEL_IDX) = Jt_v;
    // F.block<3,3>(POS_IDX,BA_IDX) = Jt_ba;

    // B.block<3,3>(PHI_IDX,NG_IDX) = Jp_nw;
    // B.block<3,3>(VEL_IDX,NA_IDX) = Jv_na;
    // B.block<3,3>(POS_IDX,NA_IDX) = Jt_na;
    // B.block<3,3>(BG_IDX,NGW_IDX) = Eigen::Matrix<double,3,3>::Identity();
    // B.block<3,3>(BA_IDX,NAW_IDX) = Eigen::Matrix<double,3,3>::Identity();
    // cout.precision(5);
#ifdef CTODW

    Eigen::Matrix<double,15,15> F = Eigen::Matrix<double,15,15>::Zero();
    Eigen::Matrix<double,15,6> G = Eigen::Matrix<double,15,6>::Zero();

    auto C_b_n = Sophus::SO3d::exp(mPreK.block<3,1>(PHI_IDX,0)).matrix();
    

    // 这里更新角度的东西使用的是
    // 只是把四元数换成了旋转矢量进行表示
    // phi_dot = omega + 0.5 * phi^ omega
    F.block<3,3>(POS_IDX,VEL_IDX) = Eigen::Matrix3d::Identity();
    F.block<3,3>(VEL_IDX,BA_IDX) = C_b_n;
    F.block<3,3>(VEL_IDX,PHI_IDX) = -1 * C_b_n * Sophus::SO3d::hat(u.mAccel);
    F.block<3,3>(PHI_IDX,PHI_IDX) = -0.5*Sophus::SO3d::hat(u.mGyro);
    F.block<3,3>(PHI_IDX,BG_IDX) = - Eigen::Matrix3d::Identity() - 0.5*Sophus::SO3d::hat(mPreK.block<3,1>(PHI_IDX,0));

    mcrF = F;

    G.block<3,3>(VEL_IDX,NA_IDX) = C_b_n;
    G.block<3,3>(PHI_IDX,NG_IDX) = F.block<3,3>(PHI_IDX,BG_IDX);
#else
    Eigen::Matrix3d R_b_n = Sophus::SO3d::exp(mPreState.block<3,1>(PHI_IDX,0)).matrix();
    Eigen::Matrix<double,15,15> F = Eigen::Matrix<double,15,15>::Zero();
    // Phi to Phi 好象是有些问题
    F.block<3,3>(PHI_IDX,PHI_IDX) = -1*Sophus::SO3d::hat(GyroNB);
    F.block<3,3>(PHI_IDX,BG_IDX) = -1 * Eigen::Matrix<double,3,3>::Identity();
    F.block<3,3>(VEL_IDX,PHI_IDX) = -1*R_b_n*Sophus::SO3d::hat(AccNB);
    F.block<3,3>(VEL_IDX,BA_IDX) = -1*R_b_n;
    F.block<3,3>(VEL_IDX,POS_IDX) = Eigen::Matrix<double,3,3>::Identity();
    
    Eigen::Matrix<double,15,12> G = Eigen::Matrix<double,15,12>::Zero();
    
    G.block<3,3>(PHI_IDX,NG_IDX) = Eigen::Matrix<double,3,3>::Identity();
    G.block<3,3>(VEL_IDX,NA_IDX) = R_b_n;
    G.block<3,3>(BG_IDX,NG_IDX) = Eigen::Matrix<double,3,3>::Identity();
    G.block<3,3>(BA_IDX,NA_IDX) = Eigen::Matrix<double,3,3>::Identity();
#endif 

   
    // cout<<"show the F matrix "<<endl<<F<<endl;
    // cout<<"show the B matrix "<<endl<<B<<endl;
    auto Q = mIN.mCov.block<6,6>(0,0);
    // mCovar = F*mCovar*F.transpose() + B*mIN.mCov*B.transpose();
    
    Eigen::Matrix<double,15,15> I = Eigen::Matrix<double,15,15>::Identity();
    mCovar = (I+F*delta_t)*mCovar*(I+F*delta_t).transpose() + G*delta_t*Q*(G*delta_t).transpose();
    
    mState.block<3,1>(PHI_IDX,0) = phi_k_prior;
    mState.block<3,1>(VEL_IDX,0) = v_k_piror;
    mState.block<3,1>(POS_IDX,0) = t_k_piror;
    mPreK = mState;
}

void EKF::Correct(const Laser & z)
{
    Eigen::Matrix<double,6,1> obser;
    obser.block<3,1>(0,0) = z.mDRot;
    obser.block<3,1>(3,0) = z.mDTra;
    
    Eigen::Matrix<double,6,15> G = Eigen::Matrix<double,6,15>::Zero();
    G.block<3,3>(3,6) = Eigen::Matrix3d::Identity();
    G.block<3,3>(0,0) = Eigen::Matrix3d::Identity();
    // cout<<"show the G" <<endl<< G <<endl;
    Eigen::Matrix<double,6,6> C = Eigen::Matrix<double,6,6>::Identity();

    // Eigen::Matrix<double,15,1> dState = mState - mPreState;
    // dState.block<3,1>(PHI_IDX,0) = (Sophus::SO3d::exp(mState.block<3,1>(PHI_IDX,0))*
    //                                 Sophus::SO3d::exp(-1*mPreState.block<3,1>(PHI_IDX,0))).log();
    // cout<<"the prior cov is "<<endl<<mCovar<<endl;


    // 方差更新
    Eigen::Matrix<double,15,6> K = mCovar*G.transpose()*(G*mCovar*G.transpose()+mLN.mCov).inverse();
    mCovar = (Eigen::Matrix<double,15,15>::Identity()-K*G)*mCovar;
    auto dx = K*(obser-G*mState);
    // 
    // dState.block<3,1>(POS_IDX,0) = Sophus::SO3d::exp(-1*mPreState.block<3,1>(PHI_IDX,0))* dState.block<3,1>(POS_IDX,0);
    // cout<<"the relative Rot/Tra is "<< endl<<(G*dState).transpose()<<endl;
    // cout<<"the postier cov is "<<endl<<mCovar<<endl;
    // cout<<"the modification value is "<<endl<<
    // (K*(obser-G*dState)).transpose()<<endl;
    mState.block<12,1>(VEL_IDX,0) = mState.block<12,1>(VEL_IDX,0) + dx.block<12,1>(VEL_IDX,0);
    Sophus::SO3d old_so3,delta_so3;
    old_so3 = Sophus::SO3d::exp(mState.block<3,1>(PHI_IDX,0));
    delta_so3 = Sophus::SO3d::exp(dx.block<3,1>(PHI_IDX,0));
    mState.block<3,1>(PHI_IDX,0) = (old_so3*delta_so3).log();
    mPreK = mState;
    // Eigen::MatrixXd Ob = obser;
    // mvOb.push_back(Ob);
    // UpdateOM();
    // cout<<"the delta position of mState is "<<endl<<dState.block<3,1>(POS_IDX,0).transpose()<<endl;
    // cout<<"the postion of mState is "<< endl<< mState.block<3,1>(POS_IDX,0).transpose()<<endl;
}

Eigen::MatrixXd EKF::GetOM()const
{
    int n = mvOM.size();
    Eigen::MatrixXd SOM(n*mvOM.front().rows(),15);
    Eigen::MatrixXd Y_all(15*n,1);
    SOM.setZero();
    Y_all.setZero();
    for(int i =0;i<n;i++)
    {
        SOM.block(mvOM.front().rows()*i,0,mvOM.front().rows(),15) = mvOM.at(i);
        for(int j=0;j<15;j++)
        Y_all.block(6*j,0,6,1) = mvOb.at(i);
    }
    Eigen::JacobiSVD<Eigen::MatrixXd> svd(SOM, Eigen::ComputeThinU | Eigen::ComputeThinV);
    Eigen::MatrixXd X;
    Eigen::MatrixXd U = svd.matrixU();
    Eigen::MatrixXd V = svd.matrixV();
    Eigen::VectorXd EV = svd.singularValues();
    std::vector<std::pair<double,int>> vEI;
    for(int i = 0; i < U.cols() ;i++)
    {
        Eigen::VectorXd x = U.col(i).transpose()*Y_all*V.col(i)/EV(i);
        int count = 0;
        double max = std::numeric_limits<double>().min();
        double ev=0;
        for(int j =0;j<15;j++)
        {
            if(x(j)>max)
            {
                count = j;
                max = x(j);
            }          
        }
        vEI.push_back(std::pair<double,int>(EV(i),count));
    }
    
    for(auto & ei:vEI)
    {
        cout<<"the singularity value is "<<ei.first<<". the corresponding index is "<< ei.second<<endl; 
    }
}

void EKF::UpdateOM()
{
    Eigen::Matrix<double,6,15> G = Eigen::Matrix<double,6,15>::Zero();
    G.block<3,3>(3,6) = Eigen::Matrix3d::Identity();
    G.block<3,3>(0,0) = Eigen::Matrix3d::Identity();
    Eigen::MatrixXd OM(15*6,15);
    OM.setZero();
    for(int i =0;i<6;i++)
    {
        Eigen::Matrix<double,15,15> tmp_F;
        tmp_F.setZero();
        for(int j=0;j<i;j++)
        {
            tmp_F = tmp_F*mcrF;
        }
        OM.block<6,15>(6*i,0) = G*tmp_F;
    }
    mvOM.push_back(OM);
}

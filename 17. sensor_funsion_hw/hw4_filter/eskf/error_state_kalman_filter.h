/*
 * @Author: Liu Weilong
 * @Date: 2020-12-04 23:13:20
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2021-02-03 15:52:00
 * @Description: 之前对于问题粗浅的理解导致现在代码写得非常难受
 * 
 *               2020.12.13 
 *               现在基本确定 ESKF 流程为
 *               1. ErrorState 、 State 双递推
 *                  ES_k+1 = (I+F*delta_t)*ES_k + G*N*delta_t 
 *                  这里的 N 哪怕期望是0, 这里也要附上一个随机值
 *                  不然ES_k+1 不会有任何修正效果
 *               2. Observeration - State 得到 ErrorState Obser 这里会存在左乘右乘的问题？
 *               3. 使用ErrorStateOb 放入 ErrorState KF 框架当中继续进行更新
 *               4. ErrorState 被更新到State 当中
 *                  IMU 的Bias 是否更新， 在GeYao的实现中，会check 一下Cov 是否收敛。 
 *               5. ErrorState 置零 然后从1 继续进行
 * 
 *
 *               
 *               现在存在的问题是如何 之前说的那个随机偏差要如何引入的问题
 *               如果不引入这个随机偏差 无法进行更新
 *               2020.12.31 进行确认之后，这里这里是可以保持0 的，更新的时候只是利用了方差
 *               对于预积分的方法
 *               类似的误差分析主要是用于获取方差
 *
 */


#include "kalman_filter_interface.h"
#include "measure_module.h"
#include "yaml-cpp/yaml.h"

using namespace std;

// #define UPDATESO3

#define SIMULATION

#define PHI_IDX 0
#define VEL_IDX 3
#define POS_IDX 6
#define BG_IDX 9
#define BA_IDX 12
#define NG_IDX 0
#define NA_IDX 3
#define NGW_IDX 6
#define NAW_IDX 9

#define I3 Eigen::Matrix3d::Identity()
#define I15 Eigen::Matrix<double,15,15>::Identity()

#define F_1ST
#define COV_CHECK

class ESKF:public EKFInterface<IMU,Laser,15>
{
    public:
    
    using MatrixF = Eigen::Matrix<double,15,15>;
    using MatrixG = Eigen::Matrix<double,15,12>;
    
    ESKF()=default;
    ESKF(const Covar & covar,const State & s,const IMUNoise & in, const LaserNoise & ln);
    // ESKF(const string & init_path);
    
    // interface
    virtual Eigen::MatrixXd GetOM()const override;
    virtual void Predict(const IMU & u) override;
    virtual void Correct(const Laser & z) override;
    ~ESKF(){}

    // internal
    void DataProcess(const IMU & rIMU, IMU & ubIMU);
    void UpdateState(const IMU & ubIMU);
    void UpdateErrorState(const IMU & ubIMU, MatrixF & F, MatrixG & G);
    void UpdateErrorStateCovarance(const MatrixF & F,const MatrixG & G);
    void UpdateErrorStateToState();

    template <int N>
    bool BuildErrorStateObs(const double * PhiObs, const double * VelObs, const double * PosiObs,
                            Eigen::Matrix<double,N,1> & Y_obs, Eigen::Matrix<double,N,15> & G);

    private:
    using EKFInterface::mState;
    using EKFInterface::mCovar;
    State mErrorState;
    Covar mErrorCov;

    IMUNoise mIN;
    LaserNoise mLN;
    IMU mpreIMU;
    IMU mpreLaser;
};

template<int N>
bool ESKF::BuildErrorStateObs(const double * PhiObs, const double * VelObs, const double * PosiObs,
                              Eigen::Matrix<double,N,1> & Y_obs, Eigen::Matrix<double,N,15> & C)
{
    Y_obs.setZero();
    C.setZero();
    // CHECK dimension
    int count = 0;
    if(PhiObs != nullptr) count ++;
    if(VelObs != nullptr) count ++;
    if(PosiObs != nullptr) count ++;
    
    if(count*3 != N || count == 0 ) 
    {
        std::cout<<"[ERROR] invalid dimension"<<std::endl;
        return false;
    }

    int idx_count = 0;
    if(PhiObs != nullptr)
    {
        C.block<3,3>(idx_count*3,PHI_IDX) = I3;
        // Y calculate
        Eigen::Map<const Eigen::Vector3d> phi_true(PhiObs);
        Eigen::Vector3d phi_nominal(mState.block<3,1>(PHI_IDX,0));
        Sophus::SO3d SO3_true = Sophus::SO3d::exp(phi_true);
        Sophus::SO3d SO3_nominal = Sophus::SO3d::exp(phi_nominal);
        Y_obs.block<3,1>(idx_count*3,0) = (SO3_nominal.inverse()*SO3_true).log();
        idx_count++;
    }
    if(VelObs != nullptr)
    {
        C.block<3,3>(idx_count*3,VEL_IDX) = I3;
        // Y calculate
        Eigen::Map<const Eigen::Vector3d> vel_true(VelObs);
        Y_obs.block<3,1>(idx_count*3,0) = vel_true - mState.block<3,1>(VEL_IDX,0); 
        idx_count++;
    }
    if(PosiObs != nullptr)
    {
        C.block<3,3>(idx_count*3,POS_IDX) = I3;
        // Y calculate
        Eigen::Map<const Eigen::Vector3d> posi_true(PosiObs);
        Y_obs.block<3,1>(idx_count*3,0) = posi_true - mState.block<3,1>(POS_IDX,0);
        idx_count++;
    }

    return true;
}
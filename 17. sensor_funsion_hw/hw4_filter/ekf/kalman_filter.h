/*
 * @Author: Liu Weilong
 * @Date: 2020-12-04 23:13:20
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2020-12-08 18:57:56
 * @Description: 之前对于问题粗浅的理解导致现在代码写得非常难受
 */
#include "kalman_filter_interface.h"
#include "measure_module.h"
#include "yaml-cpp/yaml.h"
#include "ImuTypes.h"

#define Jr(x,y,z) \
TypeTransform(RightJacobianSO3(x,y,z))
#define JrInv(x,y,z) \
TypeTransform(InverseRightJacobianSO3(x,y,z))

using namespace std;

// #define UPDATESO3

#define SIMULATION
#define CTODW

#define PHI_IDX 0
#define VEL_IDX 3
#define POS_IDX 6
#define BG_IDX 9
#define BA_IDX 12
#define NG_IDX 0
#define NA_IDX 3
#define NGW_IDX 6
#define NAW_IDX 9

class EKF:public EKFInterface<IMU,Laser,15>
{
    public:
    
    
    EKF()=default;
    EKF(const Covar & covar,const State & s,const IMUNoise & in, const LaserNoise & ln);
    // ESKF(const string & init_path);
    
    // interface
    virtual Eigen::MatrixXd GetOM()const override;
    virtual void Predict(const IMU & u) override;
    virtual void Correct(const Laser & z) override;
    ~EKF(){}

    // internal
    void UpdateOM();


    private:
    using EKFInterface::mState;
    using EKFInterface::mCovar;

    State mPreState = State::Zero();
    State mPreK = State::Zero();
    IMUNoise mIN;
    LaserNoise mLN;
    IMU mpreIMU;
    IMU mpreLaser;
    Eigen::Matrix<double,15,15> mcrF;
    vector<Eigen::MatrixXd> mvOb;
    vector<Eigen::MatrixXd> mvOM;
};


#include "ekf.h"

void EKF::addImuMeasurement(const Eigen::Vector3d &acceleration, const Eigen::Vector3d &angVel, const float &dt)
{
    cv::Point3f accel(acceleration.x(),acceleration.y(),acceleration.z());
    cv::Point3f Omega(angVel.x(),angVel.y(),angVel.z());
    imu_preintegrated_.IntegrateNewMeasurement(accel,Omega,dt);
}

void EKF::addLaserMeasurement(Eigen::Vector3d delta_rotation, Eigen::Vector3d delta_translation)
{
    // 构建优化
    buildProblem(delta_rotation,delta_translation);
    // 更新IMU
    Update();
}

auto EKF::getResult()const
{
    return make_pair(cur_state_.block(0,0,3,1),cur_state_.block(6,0,3,1));
}

Eigen::Vector3d EKF::getPosition() const
{
    return Eigen::Vector3d(pre_state_.block(6,0,3,1));
}

Eigen::Matrix<double,6,1> EKF::getBias() const
{
    return Eigen::Matrix<double,6,1>(pre_state_.block(9,0,6,1));
}

void EKF::buildProblem(Eigen::Vector3d delta_rotation_measure, Eigen::Vector3d delta_translation_measure)
{
    ceres::Problem problem;
    
    problem.AddParameterBlock(cur_state_.data(),3,new SO3PlusOnlyLocalParameterization());
    problem.AddParameterBlock(cur_state_.data()+6,3);
    // auto cf1 = EKFObserError::Create(laser_meassurement_covar_,delta_rotation_measure,delta_translation_measure);
    // problem.AddResidualBlock(cf1,NULL,cur_state_.data(),cur_state_.data()+6);

#ifdef AAA

    Eigen::Matrix<double,15,15> whole_covar = Eigen::Matrix<double,15,15>::Identity();
    whole_covar.block(0,0,9,9) = whole_covar.block(0,0,9,9)*1e-4;

    {
    cur_state_.block(0,0,3,1) = TypeTransform(IMU::LogSO3(imu_preintegrated_.GetOriginalDeltaRotation()));
    cur_state_.block(3,0,3,1) = TypeTransform(imu_preintegrated_.GetOriginalDeltaVelocity());
    Eigen::Vector3d velocity_w = pre_state_.block(3,0,3,1);
    Eigen::Vector3d so3_i_w = pre_state_.block(0,0,3,1);
    Sophus::SO3d rotatoin_w_i = Sophus::SO3d::exp(-1*so3_i_w);
    Eigen::Vector3d velocity_i = rotatoin_w_i * velocity_w;

    Eigen::Vector3d dP_whole =  TypeTransform(imu_preintegrated_.GetOriginalDeltaPosition()) +
                                velocity_i*imu_preintegrated_.dT;
    cur_state_.block(6,0,3,1) = dP_whole;
    }

    cur_state_.block(3,0,3,1) = TypeTransform(imu_preintegrated_.avgA*imu_preintegrated_.dT);
    cur_state_.block(6,0,3,1) = TypeTransform(imu_preintegrated_.avgA*imu_preintegrated_.dT*imu_preintegrated_.dT)
                                + pre_state_.block(3,0,3,1) * imu_preintegrated_.dT;
    cur_state_.block(0,0,3,1) = TypeTransform(imu_preintegrated_.avgW)*imu_preintegrated_.dT;


#else

    cur_state_.setZero();
    cur_state_.block(3,0,3,1) = TypeTransform(imu_preintegrated_.avgA*imu_preintegrated_.dT);
    cur_state_.block(6,0,3,1) = TypeTransform(imu_preintegrated_.avgA*imu_preintegrated_.dT*imu_preintegrated_.dT)
                                + pre_state_.block(3,0,3,1) * imu_preintegrated_.dT;
    cur_state_.block(0,0,3,1) = TypeTransform(imu_preintegrated_.avgW)*imu_preintegrated_.dT;
    showMat(imu_preintegrated_.C);
    Eigen::Matrix<double,15,15> whole_covar = TypeTransform(imu_preintegrated_.C);

#endif

    auto cf2 = EKFPredictError::Create(&imu_preintegrated_,whole_covar,pre_state_);
    
    problem.AddResidualBlock(cf2,NULL,cur_state_.data(),cur_state_.data()+3,
                                      cur_state_.data()+6,cur_state_.data()+9,
                                      cur_state_.data()+12);
    
    ceres::Solver::Options options;
    ceres::Solver::Summary summary;
    options.linear_solver_type = ceres::DENSE_QR;
    options.minimizer_type = ceres::TRUST_REGION;
    options.trust_region_strategy_type = ceres::DOGLEG;
    options.max_num_iterations = 100;
    
    // 这个Solve 和那个不能用的solve 看起来还是有区分度的
    ceres::Solve(options,&problem,&summary);

    cout<<summary.BriefReport()<<endl;


    // UpdateCovar(problem);
#ifdef TEST_PRINT

    Eigen::Vector3d velocity_w = pre_state_.block(3,0,3,1);
    Eigen::Vector3d so3 = pre_state_.block(0,0,3,1);
    Sophus::SO3d rotatoin_w_i = Sophus::SO3d::exp(-1*so3);
    Eigen::Vector3d velocity_i = rotatoin_w_i * velocity_w;
    cout<<"the delta P with V*dT is"<<endl<<
    (TypeTransform(imu_preintegrated_.dP)+ velocity_i * imu_preintegrated_.dT).transpose()<<endl;
    cout<<"the delta V is"<<endl<<
    TypeTransform(imu_preintegrated_.dV).transpose()<<endl;
    Eigen::Matrix3d tmp_rot_matrix= TypeTransform(imu_preintegrated_.dR);
    Eigen::AngleAxisd angle_axis(tmp_rot_matrix);
    
    cout<<"the delta R is"<<endl<<
    (angle_axis.axis()*angle_axis.angle()).transpose()<<endl;

    cout<<"the gps measure translation is "<<endl<<
    delta_translation_measure.transpose()<<endl;
    cout<<"the gps measure rotation is"<<endl<<
    delta_rotation_measure.transpose()<<endl;

    cout<<"the bias is "<<endl<<
    cur_state_.block(9,0,6,1).transpose()<<endl;
    cout<<"the delta RVP is "<<endl<<
    cur_state_.block(0,0,9,1).transpose()<<endl;

#endif

}

void EKF::Update()
{


#ifdef TEST

    Eigen::Vector3d so3_j_i = TypeTransform(IMU::LogSO3(imu_preintegrated_.dR));
    auto so3_i_w = Sophus::SO3d::exp(pre_state_.block(0,0,3,1));
    pre_state_.block(0,0,3,1) = (so3_i_w * Sophus::SO3d::exp(so3_j_i)).log();
    pre_state_.block(3,0,3,1) += so3_i_w.matrix()*TypeTransform(imu_preintegrated_.dV);
    pre_state_.block(6,0,3,1) += so3_i_w.matrix()*TypeTransform(imu_preintegrated_.dP);
    pre_state_.block(9,0,6,1) = Eigen::Matrix<double,6,1>::Zero();
    IMU::Bias new_bias(pre_state_(12,0),pre_state_(13,0),pre_state_(14,0),
                    pre_state_(9,0), pre_state_(10,0),pre_state_(11,0));
    imu_preintegrated_.Initialize(new_bias);

#else

    // 数值更新
    pre_state_.block(3,0,12,1) += cur_state_.block(3,0,12,1);
    
    auto pre_SO3 = Sophus::SO3d::exp(pre_state_.block(0,0,3,1));
    auto delta_SO3 = Sophus::SO3d::exp(cur_state_.block(0,0,3,1));
    pre_state_.block(0,0,3,1) = (pre_SO3*delta_SO3).log();

    IMU::Bias new_bias(pre_state_(12,0),pre_state_(13,0),pre_state_(14,0),
                       pre_state_(9,0), pre_state_(10,0),pre_state_(11,0));
    imu_preintegrated_.Initialize(new_bias);

#endif

    // IMU Covar 进行更新
    cv::Mat new_C = cv::Mat::zeros(15,15,CV_32F);
    for(int i=0;i<15;i++)
    for(int j=0;j<15;j++)
        new_C.at<float>(i,j)=cur_covar_(i,j);
    imu_preintegrated_.C = new_C;
    cur_state_.setZero();
}

void EKF::UpdateCovar(ceres::Problem & problem)
{
    
    ceres::Covariance::Options options_covar;
    ceres::Covariance covariance(options_covar);

    vector<pair<const double*, const double*> > covariance_blocks;

    for(int i=0;i<5;i++)
    {
        for(int j=0;j<5;j++)
        {
            covariance_blocks.push_back(make_pair(cur_state_.data()+i*3, cur_state_.data()+j*3));     
        }
    }

    covariance.Compute(covariance_blocks, &problem);

    Eigen::Matrix<double,15,15> covar = Eigen::Matrix<double,15,15>::Zero();
    Eigen::Matrix<double,3,3,Eigen::RowMajor>   element_covar = Eigen::Matrix<double,3,3>::Zero();
    for(int i=0;i<5;i++)
    {
        for(int j=0;j<5;j++)
        {
            covariance.GetCovarianceBlock(cur_state_.data()+i*3, cur_state_.data()+j*3, element_covar.data());
            covar.block(i*3,j*3,3,3)=element_covar;   
        }
    }
    cur_covar_ = covar;

    cout<<"the trace of covar is "<< covar.trace()<<endl;
}



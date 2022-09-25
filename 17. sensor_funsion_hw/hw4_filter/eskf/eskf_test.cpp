/*
 * @Author: Liu Weilong
 * @Date: 2020-12-27 10:08:35
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2020-12-27 10:57:34
 * @Description: 
 */
#include "error_state_kalman_filter.h"
#include "yaml-cpp/yaml.h"
#include "read_csv.h"
#include "omp.h"
#include "common.h"
using namespace std;

void LoadParam(IMUNoise & in,LaserNoise & ln)
{
    YAML::Node config = YAML::LoadFile("/home/lwl/workspace/3rd-test-learning/19. sensor_funsion_hw/hw4_filter/config/eskf.yaml");
    auto ImuConfig = config["IMUNoise"];
    auto laserConfig = config["LaserNoise"];
    in.mNg = ImuConfig["Ng"].as<double>();
    in.mNa = ImuConfig["Na"].as<double>();
    in.mNgwalk = ImuConfig["Ngwalk"].as<double>();
    in.mNawalk = ImuConfig["Nawalk"].as<double>();
    ln.mRotN = laserConfig["NRot"].as<double>();
    ln.mTraN = laserConfig["NTra"].as<double>();
    in.UpdateCov();
    ln.UpdateCov();
}

int main()
{
    
    // ====================================   数据提取 ====================================================
    std::string gyro_real_p = "/home/lwl/workspace/HW/gnss-ins-sim/demo_motion_def_files/imu_simulation_hw2/ref_gyro.csv";
    std::string gyro_measurement_p = "/home/lwl/workspace/HW/gnss-ins-sim/demo_motion_def_files/imu_simulation_hw2/gyro-0.csv";
    std::string accel_real_p = "/home/lwl/workspace/HW/gnss-ins-sim/demo_motion_def_files/imu_simulation_hw2/ref_accel.csv";
    std::string accel_measurement_p = "/home/lwl/workspace/HW/gnss-ins-sim/demo_motion_def_files/imu_simulation_hw2/accel-0.csv";
    std::string real_position_p = "/home/lwl/workspace/HW/gnss-ins-sim/demo_motion_def_files/imu_simulation_hw2/ref_pos.csv";
    std::string real_rotation_p = "/home/lwl/workspace/HW/gnss-ins-sim/demo_motion_def_files/imu_simulation_hw2/ref_att_quat.csv";
    
    std::vector<std::string> gyro_real_headers;
    std::vector<std::vector<double>> gyro_real;
    std::vector<std::string> gyro_meas_headers;
    std::vector<std::vector<double>> gyro_meas;
    std::vector<std::string> accel_real_headers;
    std::vector<std::vector<double>> accel_real;
    std::vector<std::string> accel_meas_headers;
    std::vector<std::vector<double>> accel_meas;
    std::vector<std::string> pos_real_headers;
    std::vector<std::vector<double>> pos_real;
    std::vector<std::string> rot_real_headers;
    std::vector<std::vector<double>> rot_real;

    std::vector<Eigen::Matrix<double,3,1>> gyro_in_vector;
    std::vector<Eigen::Matrix<double,3,1>> gyro_real_in_vector;
    std::vector<Eigen::Matrix<double,3,1>> accel_in_vector;
    std::vector<Eigen::Matrix<double,3,1>> accel_real_in_vector;
    std::vector<Eigen::Matrix<double,3,1>> pos_real_in_vector;
    std::vector<Eigen::Matrix<double,4,1>> rot_real_in_vector;
 
    omp_set_num_threads(6);

    YAML::Node config = YAML::LoadFile("/home/lwl/workspace/3rd-test-learning/19. sensor_funsion_hw/hw4_filter/config/eskf.yaml");

    int line_count = config["line_count"].as<int>();
    #pragma omp parallel sections
    {   
        #pragma omp section
        {
            if(!readCSV(gyro_real_headers,gyro_real,gyro_real_p,line_count))
            {
                std::abort();
            };
        }
        #pragma omp section
        {
            if(!readCSV(gyro_meas_headers,gyro_meas,gyro_measurement_p,line_count))
            {
                std::abort();
            };
        }
        #pragma omp section
        {
            if(!readCSV(accel_real_headers,accel_real,accel_real_p,line_count))
            {
                std::abort();
            };
        }
        #pragma omp section
        {
            if(!readCSV(accel_meas_headers,accel_meas,accel_measurement_p,line_count))
            {
                std::abort();
            };
        }
        #pragma omp section
        {
            if(!readCSV(pos_real_headers,pos_real,real_position_p,line_count))
            {
                std::abort();
            };
        }
        #pragma omp section
        {
            if(!readCSV(rot_real_headers,rot_real,real_rotation_p,line_count))
            {
                std::abort();
            };
        }
    }

    #pragma omp parallel sections 
    {
        #pragma omp section
        {
            transformIntoVector(gyro_meas,gyro_in_vector);
        }
        #pragma omp section
        {
            transformIntoVector(gyro_real,gyro_real_in_vector);
        }
        #pragma omp section
        {
            transformIntoVector(accel_meas,accel_in_vector);
        }
        #pragma omp section
        {
            transformIntoVector(accel_real,accel_real_in_vector);
        }
        #pragma omp section
        {
            transformIntoVector(pos_real,pos_real_in_vector);
        }
        #pragma omp section
        {
            transformIntoVector(rot_real,rot_real_in_vector);
        }
    }

    std::vector<Eigen::Vector3d> rot_obs_in_vector;
    rot_obs_in_vector.resize(rot_real_in_vector.size(),Eigen::Vector3d::Zero());

    
    assert(pos_real_in_vector.size() == rot_real_in_vector.size());
    
    // 数据类型转换
    #pragma omp parallel for
    for(int i=0;i<rot_real_in_vector.size();i++)
    {
        Eigen::Quaterniond tmp_q(rot_real_in_vector.at(i)(0),
                                 rot_real_in_vector.at(i)(1),
                                 rot_real_in_vector.at(i)(2),
                                 rot_real_in_vector.at(i)(3));
        Eigen::AngleAxisd angle_axis(tmp_q);
        rot_obs_in_vector[i] = angle_axis.angle()* angle_axis.axis();
    }
    
    std::vector<Eigen::Vector3d> pos_obs_in_vector;
    pos_obs_in_vector.resize(rot_real_in_vector.size(),Eigen::Vector3d::Zero());
    
    // pos 变为相对量
    #pragma omp parallel for
    for(int i =1; i<pos_real_in_vector.size();i++)
    {
        pos_obs_in_vector.at(i) = pos_real_in_vector[i]-pos_real_in_vector[i-1];
    }

    // accel 去gravity
    Eigen::Vector3d gravity_vector_w(0,0,1.0);
    gravity_vector_w = gravity_vector_w * (9.794841972265039942);

    // gyro 从角度单位变为弧度单位
    #pragma omp parallel for
    for(int i=0;i<gyro_in_vector.size();i++)
    {
        gyro_in_vector.at(i) = gyro_in_vector.at(i)/180.0*M_PI;
        // cout<< gyro_in_vector.at(i).transpose()<<endl;
        gyro_real_in_vector.at(i) = gyro_real_in_vector.at(i)/180.0*M_PI;
        // cout<< gyro_real_in_vector.at(i).transpose()<<endl;
    }

    // 先不添加噪声看一看效果 bias 的纠正效果
    // 跑一段即可
    LaserNoise ln;
    IMUNoise in;
    LoadParam(in,ln);
    
    Eigen::Matrix<double,15,1> initState = Eigen::Matrix<double,15,1>::Zero();
    Eigen::Matrix<double,15,15> initCov = Eigen::Matrix<double,15,15>::Identity() * 1e-5;
    
    // 往ESKF里面放数据

    int interval = config["interval"].as<int>();
    int count = 0;
    double hz = 100.0;
    assert(pos_obs_in_vector.size() == rot_obs_in_vector.size());
    assert(pos_obs_in_vector.size() == accel_in_vector.size());
    assert(pos_obs_in_vector.size() == gyro_in_vector.size());

    int data_size = pos_obs_in_vector.size();
    int pre_laser_index = 0;
    
    std::vector<Eigen::Vector3d> v_positions;
    

    
    ESKF ekf(initCov,initState,in,ln);

    int correct_count =1;
    for(int i =pre_laser_index;i<data_size;i++)
    {
        IMU imuMeas;
        imuMeas.mAccel = accel_in_vector.at(i);
        imuMeas.mGyro = gyro_in_vector.at(i);

        if(count == interval)
        {
            count = 0;
            Sophus::SO3d R_w_i_SO3 = Sophus::SO3d::exp(-1*rot_obs_in_vector.at(pre_laser_index));
            Sophus::SO3d R_j_w_SO3 = Sophus::SO3d::exp(rot_obs_in_vector.at(i));

            Laser laserMeas;
            laserMeas.mDRot = R_j_w_SO3.log();
            laserMeas.mDTra = pos_real_in_vector[i]-pos_real_in_vector.front();
            
            cout<<"the gps measure translation is "<<endl<<
            laserMeas.mDTra.transpose()<<endl;
            cout<<"the gps measure rotation is"<<endl<<
            laserMeas.mDRot.transpose()<<endl;

            ekf.Correct(laserMeas);
            // cout<<"this is "<<correct_count <<"times correction!"<<endl;
            // cout<<"the current covar is "<< endl << ekf.GetCovar().transpose()<<endl;
            cout<<"the current state is "<< endl << ekf.GetCurState().transpose()<<endl;
            pre_laser_index = i;
            correct_count ++;
            continue;
        }

        ekf.Predict(imuMeas);
        count++;
    }
    ekf.GetOM();
    
    
    return 0;
}


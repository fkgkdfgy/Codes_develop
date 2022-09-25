/*
 * @Author: Liu Weilong
 * @Date: 2020-11-22 10:47:26
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2021-05-23 21:42:44
 * @Description: EKF 测试代码 从gnss-ins-sim 之中提取真值加上噪声之后，
 *                           对激光得到数值进行模拟
 */

#include "kalman_filter.h"
#include "opencv2/opencv.hpp"
#include "read_csv.h"
#include "omp.h"
#include "common.h"
using namespace std;

// void LoadCSV()
// {
//     vector<string> vPath;
//     vector<string> vName;
//     vPath.push_back("/home/lwl/workspace/HW/gnss-ins-sim/demo_motion_def_files/imu_simulation_hw2/ref_gyro.csv");
//     vPath.push_back("/home/lwl/workspace/HW/gnss-ins-sim/demo_motion_def_files/imu_simulation_hw2/gyro-0.csv");
//     vPath.push_back("/home/lwl/workspace/HW/gnss-ins-sim/demo_motion_def_files/imu_simulation_hw2/ref_accel.csv");
//     vPath.push_back("/home/lwl/workspace/HW/gnss-ins-sim/demo_motion_def_files/imu_simulation_hw2/accel-0.csv");
//     vPath.push_back("/home/lwl/workspace/HW/gnss-ins-sim/demo_motion_def_files/imu_simulation_hw2/ref_pos.csv");
//     vPath.push_back("/home/lwl/workspace/HW/gnss-ins-sim/demo_motion_def_files/imu_simulation_hw2/ref_att_quat.csv");
//     vName.push_back("gyro_nominal");
//     vName.push_back("gyro_measure");
//     vName.push_back("accel_nominal");
//     vName.push_back("accel_measure");
//     vName.push_back("real_pos");
//     vName.push_back("real_atti");

//     DataCollector dc(vPath,vName,20);

// }

void LoadParam(IMUNoise & in,LaserNoise & ln)
{
    cv::FileStorage config("/home/lwl/workspace/3rd-test-learning/19. sensor_funsion_hw/hw4_filter/config/eskf.yaml",cv::FileStorage::READ);
    auto ImuConfig = config["IMUNoise"];
    auto laserConfig = config["LaserNoise"];
    in.mNg = 5e-2;
    in.mNa = 5e-2;
    in.mNgwalk = 1e-4;
    in.mNawalk = 1e-4;
    ln.mRotN = 1e-4;
    ln.mTraN = 1e-4;
    in.UpdateCov();
    ln.UpdateCov();
}



int main()
{
    
    // ====================================   数据提取 ====================================================
    std::string gyro_real_p = "/home/lwl/workspace/gnss-ins-sim/demo_saved_data/2021-05-23-21-27-44/ref_gyro.csv";
    std::string gyro_measurement_p = "/home/lwl/workspace/gnss-ins-sim/demo_saved_data/2021-05-23-21-27-44/gyro-0.csv";
    std::string accel_real_p = "/home/lwl/workspace/gnss-ins-sim/demo_saved_data/2021-05-23-21-27-44/ref_accel.csv";
    std::string accel_measurement_p = "/home/lwl/workspace/gnss-ins-sim/demo_saved_data/2021-05-23-21-27-44/accel-0.csv";
    std::string real_position_p = "/home/lwl/workspace/gnss-ins-sim/demo_saved_data/2021-05-23-21-27-44/ref_pos.csv";
    std::string real_rotation_p = "/home/lwl/workspace/gnss-ins-sim/demo_saved_data/2021-05-23-21-27-44/ref_att_quat.csv";
    
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

    cv::FileStorage config("/home/lwl/workspace/3rd-test-learning/17. sensor_funsion_hw/hw4_filter/config/eskf.yaml",cv::FileStorage::READ);

    int line_count = (int)config["line_count"];
    
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

    // #pragma omp parallel for
    // for(int i =0;i<accel_in_vector.size();i++)
    // {
    //     accel_in_vector.at(i) = accel_in_vector.at(i) + gravity_vector_w;
    //     accel_real_in_vector.at(i) = accel_real_in_vector.at(i) + gravity_vector_w;
    // }

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
    
    // 往EKF里面放数据

    int interval = (double)config["interval"];
    int count = 0;
    double hz = 100.0;
    assert(pos_obs_in_vector.size() == rot_obs_in_vector.size());
    assert(pos_obs_in_vector.size() == accel_in_vector.size());
    assert(pos_obs_in_vector.size() == gyro_in_vector.size());

    int data_size = pos_obs_in_vector.size();
    int pre_laser_index = 0;
    
    std::vector<Eigen::Vector3d> v_positions;
    

    
    EKF ekf(initCov,initState,in,ln);

    int correct_count =1;
    for(int i =pre_laser_index;i<data_size;i++)
    {
        IMU imuMeas;
        imuMeas.mAccel = accel_in_vector.at(i);
        imuMeas.mGyro = gyro_in_vector.at(i);

        ekf.Predict(imuMeas);
        count++;
    }
    
    
    return 0;
}


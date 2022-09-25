/*
 * @Author: Liu Weilong
 * @Date: 2020-11-07 17:21:47
 * @LastEditors: Liu Weilong
 * @LastEditTime: 2020-11-07 19:28:55
 * @Description:  从csv 转到 rosbag
 */

#include "csv/read_csv.h"
#include "rosbag/bag.h"
#include "sensor_msgs/Imu.h"
#include "omp.h"

#include <string>
#include <vector>

using namespace std;

int main(int argc,char **argv)
{
  
    rosbag::Bag bag_output;
    bag_output.open("/home/lwl/workspace/PublicData/imu_simulation/imu_simulation_allen.bag",rosbag::bagmode::Write);
    std::string gyro_csv_path="/home/lwl/workspace/HW/gnss-ins-sim/demo_motion_def_files/imu_simulation_hw/gyro-0.csv";
    std::string acc_csv_path ="/home/lwl/workspace/HW/gnss-ins-sim/demo_motion_def_files/imu_simulation_hw/accel-0.csv";
    std::string time_csv_path="/home/lwl/workspace/HW/gnss-ins-sim/demo_motion_def_files/imu_simulation_hw/time.csv";
    std::vector<string> path_array;
    path_array.push_back(gyro_csv_path);
    path_array.push_back(acc_csv_path);
    path_array.push_back(time_csv_path);
    std::vector<vector<vector<double>>> data_array;
    std::vector<vector<std::string>> headers_array;
    headers_array.resize(path_array.size());
    data_array.resize(path_array.size());

    omp_set_num_threads(3);
    #pragma omp parallel for
    for(int i =0;i<path_array.size();i++)
    {
        readCSV(headers_array.at(i),data_array.at(i),path_array.at(i));
    }

    unsigned int min_size = numeric_limits<unsigned int>().max();
    for(auto & data:data_array)
    {
        for(auto & info:data)
        {
            min_size = min(min_size,static_cast<unsigned int>(info.size()));
        }
    }

    omp_set_num_threads(6);
    const auto & gyro_info_array = data_array.at(0);
    const auto & acc_info_array = data_array.at(1);
    const auto & time_array = data_array.at(2);
    std::vector<sensor_msgs::Imu> imu_info_array;
    imu_info_array.resize(min_size);
    #pragma omp parallel for
    for(int i =0;i<min_size;i++)
    {
        sensor_msgs::Imu tmp_imu_info;
        tmp_imu_info.header.stamp = ros::Time(time_array.front()[i]+10);
        tmp_imu_info.angular_velocity.x = gyro_info_array.at(0)[i];
        tmp_imu_info.angular_velocity.y = gyro_info_array.at(1)[i];
        tmp_imu_info.angular_velocity.z = gyro_info_array.at(2)[i];
        tmp_imu_info.linear_acceleration.x = acc_info_array.at(0)[i];
        tmp_imu_info.linear_acceleration.y = acc_info_array.at(1)[i];
        tmp_imu_info.linear_acceleration.z = acc_info_array.at(2)[i];
        imu_info_array.at(i) = tmp_imu_info;
    }

    for(auto & imu_info:imu_info_array)
    {

        bag_output.write("imu",imu_info.header.stamp,imu_info);
    }
    bag_output.close();
}
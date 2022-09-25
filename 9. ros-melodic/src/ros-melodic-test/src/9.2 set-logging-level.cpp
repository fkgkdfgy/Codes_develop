/*
 * @Author: Liu Weilong
 * @Date: 2020-09-16 09:42:32
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2020-09-16 09:44:55
 * @FilePath: /3rd-test-learning/9. ros-melodic/src/ros-melodic-test/src/9.2 set-logging-level.cpp
 * @Description: 设置log 等级
 */

#include <ros/ros.h>
#include <ros/console.h>


int main(int argc, char const *argv[])
{
    if( ros::console::set_logger_level(ROSCONSOLE_DEFAULT_NAME, ros::console::levels::Debug) ) {
    ros::console::notifyLoggerLevelsChanged();
    }

    ROS_INFO("AAAAAAAAAA");
    ROS_DEBUG("BBBBBBBBB");
    ROS_ERROR("CCCCCCC");
    ROS_FATAL("DDDDDDDDD");

    return 0;
}

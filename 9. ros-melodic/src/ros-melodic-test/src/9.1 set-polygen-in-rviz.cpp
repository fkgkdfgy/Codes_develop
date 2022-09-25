/*
 * @Author: Liu Weilong
 * @Date: 2020-09-16 08:35:59
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2020-09-24 10:33:02
 * @FilePath: /3rd-test-learning/9. ros-melodic/src/ros-melodic-test/src/9.1 set-polygen-in-rviz.cpp
 * @Description: 用于测试在rviz上面设置多边形的功能
 */
#include <vector>
#include "ros/ros.h"
#include "visualization_msgs/Marker.h"
#include "geometry_msgs/PointStamped.h"

// 这个实际上是多边形的显示函数
void setpolygen()
{
    vector<geometry_msgs::PointStamped> Points_;
    ROS_DEBUG_COND(!close_," in vizPubCb");
    if(Points_.size()>1)
    {
        
        visualization_msgs::Marker marker;
        marker.header.frame_id = "/map";
        marker.header.stamp = ros::Time::now();
        marker.ns = "marker_click_points";
        marker.id = 0;
        marker.type = visualization_msgs::Marker::LINE_LIST;
        marker.action = visualization_msgs::Marker::ADD;
        marker.pose.orientation.x = 0.0;
        marker.pose.orientation.y = 0.0;
        marker.pose.orientation.z = 0.0;
        marker.pose.orientation.w = 1.0;
        marker.pose.position.x = 0;
        marker.scale.x = 0.05;

        if(close_)
        {
            marker.color = polygenCloseColor_;
        }else{
            marker.color = polygenUncloseColor_;
        }
        
        marker.frame_locked = true;

        auto tmp_p = Points_.front().point;
        marker.pose.position.x = tmp_p.x;
        marker.pose.position.y = tmp_p.y;
        marker.pose.position.z = tmp_p.z;

        marker.pose.position.x = 0;
        marker.pose.position.y = 0;
        marker.pose.position.z = 0;

        
        for (int i = 0; i < Points_.size()-1; ++i)
        {
            marker.points.push_back(Points_[i].point);
            marker.points.push_back(Points_[i+1].point);
        }
        markerPub_.publish(marker);
    }
}
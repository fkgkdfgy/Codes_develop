/*
 * @Author: Liu Weilong
 * @Date: 2020-09-16 09:55:41
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2020-10-16 11:23:54
 * @FilePath: /3rd-test-learning/9. ros-melodic/src/ros-melodic-test/src/9.3 visualizetion_msgs-test.cpp
 * @Description: 用于测试 ros 显示
 */

#include <ros/ros.h>
#include <ros/timer.h>
#include <tf/tf.h>
#include <tf/transform_broadcaster.h>
#include <visualization_msgs/Marker.h>
#include <geometry_msgs/PointStamped.h>
#include <std_msgs/ColorRGBA.h>
#include <iostream>
#include <vector>
using namespace std;


// 用于显示球体
class VizTestSpheres
{
    public:

    VizTestSpheres():nh_(){
        pointSub_ = nh_.subscribe("/clicked_point",3,&VizTestSpheres::pointCb,this);
        timer_ = nh_.createTimer(ros::Duration(0.3),boost::bind(&VizTestSpheres::vizPubCb,this));
        markerPub_ = nh_.advertise<visualization_msgs::Marker>("/Marker_show",1);
        pointsPool_.clear();

        for(int i =0; i<10;i++)
        {
            for(int j =0 ;j<10;j++)
            {
                geometry_msgs::PointStamped tmp_p;
                tmp_p.header.frame_id = "/map";
                tmp_p.point.z = 0;
                tmp_p.point.x = float(i);
                tmp_p.point.y = float(j);
                pointsPool_.push_back(tmp_p);
            }
        }


        ROS_INFO("viztest initialzation");
    }

    void pointCb(geometry_msgs::PointStampedConstPtr msg)
    {
        ROS_INFO("received a Point %d ",pointsPool_.size());
        pointsPool_.push_back(*msg);
    }

    void vizPubCb()
    {
        ROS_INFO(" in vizPubCb");
        if(!pointsPool_.empty())
        {
            visualization_msgs::Marker marker;
            marker.header.frame_id = pointsPool_.front().header.frame_id;
            marker.header.stamp = ros::Time::now();
            marker.ns = "marker_test_sphere_list";
            marker.id = 0;
            marker.type = visualization_msgs::Marker::SPHERE_LIST;
            marker.action = visualization_msgs::Marker::ADD;
            marker.pose.orientation.x = 0.0;
            marker.pose.orientation.y = 0.0;
            marker.pose.orientation.z = 0.0;
            marker.pose.orientation.w = 1.0;
            marker.pose.position.x = 0;
            marker.scale.x = 4;
            marker.scale.y = 4;
            marker.scale.z = 4;
            marker.color.r = 0.0;
            marker.color.g = 1.0;
            marker.color.b = 0.0;
            marker.color.a = 1.0;
            marker.frame_locked = true;

            std_msgs::ColorRGBA tmp_c;
            tmp_c.a = 1.0;
            tmp_c.g = 1.0;
            tmp_c.r = 0;
            tmp_c.b = 0;
            for(auto point:pointsPool_)
            {               
                marker.colors.push_back(tmp_c);
                marker.points.push_back(point.point);
            }

            markerPub_.publish(marker);
        }


    }
    private:
    
    ros::NodeHandle  nh_;
    ros::Subscriber pointSub_;
    ros::Publisher markerPub_; 
    ros::Timer timer_;
    vector<geometry_msgs::PointStamped> pointsPool_;
};

// 用于显示线
class VizTestLines
{
    public:

    VizTestLines():nh_(){
        pointSub_ = nh_.subscribe("/clicked_point",3,&VizTestLines::pointCb,this);
        timer_ = nh_.createTimer(ros::Duration(0.3),boost::bind(&VizTestLines::vizPubCb,this));
        markerPub_ = nh_.advertise<visualization_msgs::Marker>("/Marker_show_lines",0);
        pointsPool_.clear();

        for(int i =0; i<10;i++)
        {
            for(int j =0 ;j<10;j++)
            {
                geometry_msgs::PointStamped tmp_p;
                tmp_p.header.frame_id = "/map";
                tmp_p.point.z = 0;
                tmp_p.point.x = float(i);
                tmp_p.point.y = float(j);
                pointsPool_.push_back(tmp_p);
            }
        }


        ROS_INFO("viztest initialzation");
    }

    void pointCb(geometry_msgs::PointStampedConstPtr msg)
    {
        ROS_INFO("received a Point %d ",pointsPool_.size());
        pointsPool_.push_back(*msg);
    }

    void vizPubCb()
    {
        ROS_INFO(" in vizPubCb");
        if(!pointsPool_.empty())
        {
            int count = 10;
            visualization_msgs::Marker marker;
            marker.header.frame_id = "/map";
            marker.header.stamp = ros::Time::now();
            marker.ns = "marker_test_line_list";
            marker.id = 0;
            marker.type = visualization_msgs::Marker::LINE_LIST;
            marker.action = visualization_msgs::Marker::ADD;
            marker.pose.position.x = 0.0;
            marker.pose.position.y = 0.0;
            marker.pose.position.z = 0.0;
            marker.pose.orientation.x = 0.0;
            marker.pose.orientation.y = 0.0;
            marker.pose.orientation.z = 0.0;
            marker.pose.orientation.w = 1.0;
            marker.pose.position.x = 0;
            marker.scale.x = 0.3;
            marker.color.r = 1.0;
            marker.color.a = 1.0;
            marker.frame_locked = true;

            for (int i = 0; i <= count; ++i)
            {
                geometry_msgs::Point p1, p2;
                p1.x = 0;
                p1.y = (i - count / 2) * 2;
                p1.z = 0;
                p2.x = 3;
                p2.y = (i - count / 2) * 2;
                p2.z = 0;
                marker.points.push_back(p1);
                marker.points.push_back(p2);
            }
            markerPub_.publish(marker);
        }


    }
    private:
    
    ros::NodeHandle  nh_;
    ros::Subscriber pointSub_;
    ros::Publisher markerPub_; 
    ros::Timer timer_;
    vector<geometry_msgs::PointStamped> pointsPool_;
};

// 用于将在rviz 上面点击的点 使用线段 将他们连接起来
class VizTestForClickPoint
{
    public:

    VizTestForClickPoint():nh_(){
        pointSub_ = nh_.subscribe("/clicked_point",3,&VizTestForClickPoint::pointCb,this);
        timer_ = nh_.createTimer(ros::Duration(0.3),boost::bind(&VizTestForClickPoint::vizPubCb,this));
        markerPub_ = nh_.advertise<visualization_msgs::Marker>("/Marker_show_clickPoint_outline",0);
        pointsPool_.clear();

        ROS_INFO("viztest initialzation");
    }

    void pointCb(geometry_msgs::PointStampedConstPtr msg)
    {
        ROS_INFO("received a Point %d ",pointsPool_.size());
        pointsPool_.push_back(*msg);
    }

    void vizPubCb()
    {
        ROS_INFO(" in vizPubCb");
        if(pointsPool_.size()>1)
        {
            int count = 10;
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

            marker.color.b = 1.0;
            marker.color.a = 1.0;
            marker.frame_locked = true;

            auto tmp_p = pointsPool_.front().point;
            marker.pose.position.x = tmp_p.x;
            marker.pose.position.y = tmp_p.y;
            marker.pose.position.z = tmp_p.z;

            marker.pose.position.x = 0;
            marker.pose.position.y = 0;
            marker.pose.position.z = 0;

            
            for (int i = 0; i < pointsPool_.size()-1; ++i)
            {
                marker.points.push_back(pointsPool_[i].point);
                marker.points.push_back(pointsPool_[i+1].point);
            }
            markerPub_.publish(marker);
        }


    }
    private:
    
    ros::NodeHandle  nh_;
    ros::Subscriber pointSub_;
    ros::Publisher markerPub_; 
    ros::Timer timer_;
    vector<geometry_msgs::PointStamped> pointsPool_;
};


void frameCallback(const ros::TimerEvent& /*unused*/)
{
  static uint32_t counter = 0;

  static tf::TransformBroadcaster br;
  tf::Transform t;

  t.setOrigin(tf::Vector3(0.0, 0.0, (counter % 1000) * 0.01));
  t.setRotation(tf::Quaternion(0.0, 0.0, 0.0, 1.0));
  br.sendTransform(tf::StampedTransform(t, ros::Time::now(), "base_link", "map"));

  ++counter;
}

int main(int argc, char **argv)
{
    ros::init(argc,argv,"viz_test");

    ros::NodeHandle nh_;
    ros::Timer frame_timer = nh_.createTimer(ros::Duration(0.01), frameCallback);
    
    VizTestSpheres viztest_;
    
    ros::spin();

    return 0;
}

#include <iostream>
#include <vector>

#include "../include/test_ros2/prototype_targets_printer.hpp"


int main(int argc, char** argv)
{
    rclcpp::init(argc,argv);
    // rclcpp::Node::SharedPtr node_ = rclcpp::Node::make_shared("target_printer");
    // auto publisher_ = node_->create_publisher<geometry_msgs::msg::PointStamped>("targets_",3);
    // // auto sub_ = node_->create_subscription<std_msgs::msg::Empty>("print_targets",3,std::bind())
    // rclcpp::Rate rate_(0.5);
    

    // std::vector<geometry_msgs::msg::PointStamped> targets_;
    
    // targets_.reserve(10);
    
    // for (int i = 0; i<10;i++)
    // {   
    //     geometry_msgs::msg::PointStamped target_temp_;  
    //     target_temp_.header.frame_id="map";
    //     target_temp_.point.x=float(i);
    //     target_temp_.point.y=0.0;
    //     targets_.push_back(target_temp_);
    // }

    // while(rclcpp::ok())
    // {
        
    //     for(auto element_ :targets_)
    //     publisher_->publish(element_);
    //     rate_.sleep();
    // }

    TargetsPrinter tp_;
    tp_.run();

    return 0;
}


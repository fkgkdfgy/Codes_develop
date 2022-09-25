#include <cstdio>
#include <memory>
#include <string>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/empty.hpp"



int main(int argc, char ** argv)
{
  
  rclcpp::init(argc,argv);
  auto nh_ = std::make_shared<rclcpp::Node>("test_pub_node_");
  auto test_pub_ = nh_->create_publisher<std_msgs::msg::Empty>("test_message_",5);
  rclcpp::Rate rate(10);
  while(rclcpp::ok())
  {
      std_msgs::msg::Empty temp;
      test_pub_->publish(temp);
      rate.sleep();
  }

  return 0;
}

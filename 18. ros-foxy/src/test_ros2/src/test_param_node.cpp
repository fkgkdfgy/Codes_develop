#include <cstdio>
#include <memory>
#include <string>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/empty.hpp"



int main(int argc, char ** argv)
{
  
  rclcpp::init(argc,argv);
  auto nh_ = std::make_shared<rclcpp::Node>("_");

  nh_->declare_parameter("test_string");
  std::string temp;
  nh_->get_parameter("test_string",temp);
  RCLCPP_INFO(nh_->get_logger(),"%s",temp.c_str());
  RCLCPP_INFO(nh_->get_logger(),"%s",argv[1]);
  printf("hello world test_ros2 package\n");

  return 0;
}

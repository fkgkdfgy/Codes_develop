/*
 * @Author: Liu Weilong
 * @Date: 2020-10-21 11:26:28
 * @LastEditors: Liu Weilong 
 * @LastEditTime: 2020-10-21 17:08:09
 * @FilePath: /3rd-test-learning/20. ros-foxy/src/test_ros2/src/test_remap_sub_node.cpp
 * @Description: 
 */
#include <cstdio>
#include <memory>
#include <string>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/empty.hpp"



class sub_node_: public rclcpp::Node
{
  public:
  sub_node_():Node("test_sub_"){}
  void CallBack(std_msgs::msg::Empty::SharedPtr /*temp*/) const 
  {
    RCLCPP_INFO(get_logger(),"Received a Message!!!");
  }
}
;

int main(int argc, char ** argv)
{
  rclcpp::init(argc,argv);

  //使用类成员函数作为回调函数
  sub_node_ temp_node_;
  auto test_sub_ = temp_node_.create_subscription<std_msgs::msg::Empty>
                              ("aaaa",5,bind(&sub_node_::CallBack,&temp_node_,std::placeholders::_1));

  rclcpp::spin(temp_node_.get_node_base_interface());

  return 0;
}

#include <iostream>
#include <string>
#include "rclcpp/rclcpp.hpp"
#include "rcl_interfaces/msg/parameter_event.hpp"

// ros param 测试 目前存在问题
// 暂不编译



void ParameterEventCallBack(const rcl_interfaces::msg::ParameterEvent::SharedPtr temp) 
{

}


int main(int argc,char** argv)
{
    rclcpp::init(argc,argv);
    auto nh_ = rclcpp::Node::make_shared("test_rqt_dynamic_reconfiture");
    std::string name_ ="LWL";
    std::string tmp_name_;
    float age_ = 23.4;
    float tmp_age_;
    // declare 内部有保存数据类型的ParameterValue 所以可以直接当作参数传入
    nh_->declare_parameter("name",name_);
    nh_->declare_parameter("age",age_);
    
    //为之前ros2 param 无法使用的情况进行的部分更改
    

    rclcpp::Rate rate_(5);
    while(rclcpp::ok())
    {
        nh_->get_parameter("name",tmp_name_);
        nh_->get_parameter("age",tmp_age_);
        RCLCPP_INFO(nh_->get_logger(),"the name is %s",tmp_name_.c_str());
        RCLCPP_INFO(nh_->get_logger(),"the age is %f",tmp_age_);
    }

    return 0;
}
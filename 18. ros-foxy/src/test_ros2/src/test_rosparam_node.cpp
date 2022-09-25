#include <iostream>
#include <memory>
#include <string>
#include "rclcpp/rclcpp.hpp"

using std::string;

// 测试通过

int main(int argc,char ** argv)
{
    rclcpp::init(argc,argv);

    auto nh_ = std::make_shared<rclcpp::Node>("test_rosparam_node_");
    
    nh_->declare_parameter("name");
    nh_->declare_parameter("sex");
    nh_->declare_parameter("address");
    nh_->declare_parameter("phone_number");
    nh_->declare_parameter("personal_information.name");
    nh_->declare_parameter("personal_information.sex");

    string frame="personal_information";
    string name_,sex_,address_;
    float phone_number_;
    
    // 测试结果是只读取 符合node 名字下面定义的parameters
    nh_->get_parameter(frame + "."+"name",name_);
    nh_->get_parameter(frame + "."+"sex",sex_);
    nh_->get_parameter("address",address_);
    nh_->get_parameter("phone_number",phone_number_);

    RCLCPP_INFO(nh_->get_logger(),"the name is %s",name_.c_str());
    RCLCPP_INFO(nh_->get_logger(),"the sex is %s",sex_.c_str());
    RCLCPP_INFO(nh_->get_logger(),"the address is %s",address_.c_str());
    RCLCPP_INFO(nh_->get_logger(),"the phone_number is %f",phone_number_);




    return 0;
}
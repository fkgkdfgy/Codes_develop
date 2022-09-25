#include <iostream>
#include <string>
#include "rclcpp/rclcpp.hpp"
#include "rclcpp/publisher.hpp"
#include "std_msgs/msg/bool.hpp"

// 主要是为了检验同一个.cpp 不同的node 名字
// 结果就是ros2 node list 看起来是一个node 但是从topic 的publisher 来看是有两个node
// 目前看来结果还是失败的。要之后继续进行测试

class Test_1to2 :public rclcpp::Node
{
    public:
    Test_1to2():Node("test_1to2_1",rclcpp::NodeOptions())
    {}
    ~Test_1to2(){}

    void run();
    private:
    rclcpp::Node::SharedPtr node_;
    rclcpp::Publisher<std_msgs::msg::Bool>::SharedPtr p_1_;
    rclcpp::Publisher<std_msgs::msg::Bool>::SharedPtr p_2_;
};

void Test_1to2::run()
{
    // 设置第二节点
    auto options_ = rclcpp::NodeOptions().arguments({"--ros-args","-r","_node:=test_1to2_2","--"});
    node_ = rclcpp::Node::make_shared("_",options_);
    rclcpp::Rate rate(5);
    p_1_ = create_publisher<std_msgs::msg::Bool>("p_1_",5);
    p_2_ = node_->create_publisher<std_msgs::msg::Bool>("p_2_",5);
    std_msgs::msg::Bool p_1_content,p_2_content;
    p_1_content.data = false;
    p_2_content.data = true;
    while(rclcpp::ok())
    {
        p_1_ ->publish(p_1_content);
        p_2_ ->publish(p_2_content);
        rate.sleep();
    }

}


int main(int argc,char** argv)
{
    rclcpp::init(argc,argv);
    Test_1to2 temp;
    temp.run();

    return 0;
}
#include "rclcpp/rclcpp.hpp"
#include "eungoo/mytopic.hpp"


subscriberNode::subscriberNode() : Node("subscriber"){ 
    mycpp_subscriber_=this->create_subscription<std_msgs::msg::String>("eung",10, std::bind(&subscriberNode::topic_callback, this, std::placeholders::_1));
}

void subscriberNode::topic_callback(const std_msgs::msg::String::SharedPtr msg){
    RCLCPP_INFO(this->get_logger(), "Received message: '%s'",msg->data.c_str());
}
int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<subscriberNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
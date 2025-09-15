#include "rclcpp/rclcpp.hpp"
#include "eungoo/mytopic.hpp"
#include <chrono>
#include "geometry_msgs/msg/twist.hpp"

using namespace std::chrono_literals;


publisherNode::publisherNode() : Node("publisher"){
    mycpp_publisher_=this->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel",10);
    timer1_=this->create_wall_timer(100ms,std::bind(&publisherNode::timer_callback,this));
    timer2_=this->create_wall_timer(100ms,std::bind(&publisherNode::get_keyboard,this));
}

void publisherNode::get_keyboard(){
    scanf("%c",&inputkey);
    if(inputkey == 'w') {
        shape->make_triangle();   // shape 객체를 접근할 수 있어야 함
    }
    //if(inputkey==same)count++;
    //else count=0;
    //same=inputkey;
    
}

void publisherNode::get_xz(int get_x, int get_z){
     x_val=get_x;
     z_val=get_z;
}

void publisherNode::timer_callback(){
    auto msg=geometry_msgs::msg::Twist();
    msg.linear.x = x_val;
    msg.angular.z = z_val;
    mycpp_publisher_->publish(msg);
}

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node  = std::make_shared<publisherNode>();
    auto shape = std::make_shared<make_shape>(node);
    
    node->shape = shape;
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
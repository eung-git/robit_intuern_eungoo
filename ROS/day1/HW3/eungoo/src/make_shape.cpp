#include "rclcpp/rclcpp.hpp"
#include "eungoo/mytopic.hpp"
#include <chrono>
#include "geometry_msgs/msg/twist.hpp"

using namespace std::chrono_literals;


make_shape::make_shape(std::shared_ptr<publisherNode> pub): Node("shapenode"), pubNode(pub) {
    timer_=this->create_wall_timer(100ms,std::bind(&make_shape::count_up,this));
    count = 0;
}

void make_shape::make_triangle(){
    if(count<6)pubNode->get_xz(0,1);
    if(count>6&&count<21)pubNode->get_xz(1,0);
    if(count>21&&count<36)pubNode->get_xz(0,1);
    if(count>36&&count<51)pubNode->get_xz(1,0);
    if(count>51&&count<57)pubNode->get_xz(0,1);
    if(count>57&&count<72)pubNode->get_xz(1,0);
    pubNode->get_xz(0,0);
    count=0;
}
void make_shape::make_square(){
    RCLCPP_INFO(this->get_logger(), "삼각형 그리기 실행");
}
void make_shape::make_star(){
    RCLCPP_INFO(this->get_logger(), "삼각형 그리기 실행");
}
void make_shape::make_circle(){
    RCLCPP_INFO(this->get_logger(), "삼각형 그리기 실행");
}
void make_shape::count_up(){
    count++;
}



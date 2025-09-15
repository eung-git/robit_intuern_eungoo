#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "geometry_msgs/msg/twist.hpp"

class publisherNode;

class make_shape : public rclcpp::Node{
private:
    rclcpp::TimerBase::SharedPtr timer_;
    std::shared_ptr<publisherNode> pubNode;
    int count=0;
public:
    make_shape(std::shared_ptr<publisherNode> pub);
    void make_triangle();
    void make_square();
    void make_star();
    void make_circle();
    void count_up();
    make_shape();
};
class publisherNode : public rclcpp::Node
{
private:
    rclcpp::TimerBase::SharedPtr timer1_,timer2_;
    void timer_callback();
    void get_keyboard();
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr mycpp_publisher_;
public:
    char inputkey='\0';
    char same='\0';
    float x_val=0;
    float z_val=0;
    publisherNode();
    void get_xz(int get_x, int get_z);
    std::shared_ptr<class make_shape> shape;
};


class subscriberNode : public rclcpp::Node
{

private:
    void topic_callback(const std_msgs::msg::String::SharedPtr msg);
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr mycpp_subscriber_;
public:
    subscriberNode();
};





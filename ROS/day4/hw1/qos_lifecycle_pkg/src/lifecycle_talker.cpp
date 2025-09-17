#include <chrono>
#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "rclcpp_lifecycle/lifecycle_node.hpp"
#include "qos_lifecycle_pkg/qos_lifecycle.hpp"

using namespace std::chrono_literals;

MinimalPublisher::MinimalPublisher(): rclcpp_lifecycle::LifecycleNode("minimal_publisher"){
    rclcpp::QoS qos_profile(10);
    qos_profile.reliability(RMW_QOS_POLICY_RELIABILITY_RELIABLE);
    publisher_ = this->create_publisher<std_msgs::msg::String>("eung", qos_profile);
    timer_ = this->create_wall_timer(1s, std::bind(&MinimalPublisher::timer_callback, this));
  }
  //on_configure 상태로 전환하는 함수: 메모리 및 변수 선언만 진행
rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn MinimalPublisher::on_configure(const rclcpp_lifecycle::State &)
  {
    RCLCPP_INFO(this->get_logger(), "on_configure()");
    msg.data="on_configure";
   
    return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
  }
  //on_activate 상태로 전환하는 함수: 저장된 메모리를 전송하는 등의 실제 동작을 수행
rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn MinimalPublisher::on_activate(const rclcpp_lifecycle::State &)
  {
    publisher_->on_activate();
    RCLCPP_INFO(this->get_logger(), "on_activate()");
    msg.data="on_activate";
   
    return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
  }
  //on_deactivate 상태로 전환하는 함수: on_activate 상태에서 수행되던 동작들을 정지
rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn MinimalPublisher::on_deactivate(const rclcpp_lifecycle::State &)
  {
    publisher_->on_deactivate();
    RCLCPP_INFO(this->get_logger(), "on_deactivate()");
    msg.data="on_deactivate";
   
    return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
  }
  //on_cleanup 상태로 전환하는 함수: 저장된 메모리를 삭제
rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn MinimalPublisher::on_cleanup(const rclcpp_lifecycle::State &)
  {
    publisher_.reset();
    timer_.reset();
    RCLCPP_INFO(this->get_logger(), "on_cleanup()");
    msg.data="on_cleanup";
    
    return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
  }


void MinimalPublisher::timer_callback()
{
    publisher_->publish(msg);
}

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<MinimalPublisher>();
  rclcpp::spin(node->get_node_base_interface());
  rclcpp::shutdown();
  return 0;
}

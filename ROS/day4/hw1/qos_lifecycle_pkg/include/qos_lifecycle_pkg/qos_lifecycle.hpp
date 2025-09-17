#include <chrono>
#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "rclcpp_lifecycle/lifecycle_node.hpp"
#include "rclcpp_lifecycle/lifecycle_publisher.hpp"

using namespace std::chrono_literals;

class MinimalPublisher : public rclcpp_lifecycle::LifecycleNode
{
public:
//생성자: 퍼블리셔, 토픽 선언 및 타이머 초기화
  MinimalPublisher();
//on_configure 상태로 전환하는 함수: 메모리 및 변수 선언만 진행
  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn on_configure(const rclcpp_lifecycle::State &);
//on_activate 상태로 전환하는 함수: 저장된 메모리를 전송하는 등의 실제 동작을 수행
  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn on_activate(const rclcpp_lifecycle::State &);
//on_deactivate 상태로 전환하는 함수: on_activate 상태에서 수행되던 동작들을 정지
  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn on_deactivate(const rclcpp_lifecycle::State &);
//on_cleanup 상태로 전환하는 함수: 저장된 메모리를 삭제
  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn on_cleanup(const rclcpp_lifecycle::State &);

private:
    std_msgs::msg::String msg;
    void timer_callback();

    std::shared_ptr<rclcpp_lifecycle::LifecyclePublisher<std_msgs::msg::String>> publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
    size_t count_;
};
class MinimalSubscriber : public rclcpp::Node
{
  public:
    MinimalSubscriber();
  private:
    void topic_callback(const std_msgs::msg::String::SharedPtr msg) const;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
};


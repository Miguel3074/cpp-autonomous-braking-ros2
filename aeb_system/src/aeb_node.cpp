#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float32.hpp"

class AebNode : public rclcpp::Node{
private:

    rclcpp::Subscription<std_msgs::msg::Float32>::SharedPtr subscriber_;

public:
    AebNode(): Node("aeb_node"){
        RCLCPP_INFO(this->get_logger(), "AEB Node has been started.");
        this->subscriber_ = this->create_subscription<std_msgs::msg::Float32>(
            "radar_data",
            10,
            std::bind(&AebNode::distance_callback, this, std::placeholders::_1)
        );
    };

    void distance_callback(const std_msgs::msg::Float32::SharedPtr msg) {
        float distancia_atual = msg->data;
        RCLCPP_INFO(this->get_logger(), "Received distance: %.2f", distancia_atual);
    };

};

int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    auto aeb_node = std::make_shared<AebNode>();
    rclcpp::spin(aeb_node);
    rclcpp::shutdown();
    return 0;
}
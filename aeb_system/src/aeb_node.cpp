#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/vector3.hpp"

class AebNode : public rclcpp::Node{
private:

    rclcpp::Subscription<geometry_msgs::msg::Vector3>::SharedPtr subscriber_;
    double ttc_threshold_ = this->declare_parameter<double>("ttc_threshold", 2.7);

public:
    AebNode(): Node("aeb_node"){
        RCLCPP_INFO(this->get_logger(), "AEB Node has been started.");
        this->subscriber_ = this->create_subscription<geometry_msgs::msg::Vector3>(
            "radar_data",
            10,
            std::bind(&AebNode::distance_callback, this, std::placeholders::_1)
        );
    };

    void distance_callback(const geometry_msgs::msg::Vector3::SharedPtr msg) {
        float distance = msg->x;
        float relative_speed = msg->y;

        if (relative_speed >= 0) {
            RCLCPP_INFO(this->get_logger(), "Relative speed is zero or negative, no collision expected.");
            return;
        }
        RCLCPP_INFO(this->get_logger(), "Received distance: %.2f", distance);
        RCLCPP_INFO(this->get_logger(), "Received relative speed: %.2f", relative_speed);

        float closing_speed = -relative_speed;
        float time_to_collision = distance / closing_speed;

        RCLCPP_INFO(this->get_logger(), "Time to collision: %.2f", time_to_collision);

        if (time_to_collision <= this->ttc_threshold_) {
            RCLCPP_ERROR(this->get_logger(), "Collision imminent, activating brakes!");
        }
    };

};

int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    auto aeb_node = std::make_shared<AebNode>();
    rclcpp::spin(aeb_node);
    rclcpp::shutdown();
    return 0;
}
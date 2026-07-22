#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/vector3.hpp"
#include "std_msgs/msg/bool.hpp"

class AebNode : public rclcpp::Node{
private:

    rclcpp::Subscription<geometry_msgs::msg::Vector3>::SharedPtr subscriber_;
    rclcpp::Publisher<std_msgs::msg::Bool>::SharedPtr aeb_pub_;
    double ttc_threshold_;

public:
    AebNode(): Node("aeb_node"){
        this->declare_parameter<double>("ttc_threshold", 2.7);
        this->ttc_threshold_ = this->get_parameter("ttc_threshold").as_double();

        RCLCPP_INFO(this->get_logger(), "AEB Node has been started.");

        this->subscriber_ = this->create_subscription<geometry_msgs::msg::Vector3>(
            "sensor_data",
            10,
            std::bind(&AebNode::distance_callback, this, std::placeholders::_1)
        );

        this->aeb_pub_ = this->create_publisher<std_msgs::msg::Bool>("aeb_status", 10);
    };

    void distance_callback(const geometry_msgs::msg::Vector3::SharedPtr msg) {
        float distance = msg->x;
        float relative_speed = msg->y;

        std_msgs::msg::Bool aeb_msg;
        aeb_msg.data = false;

        if (relative_speed >= 0) {
            this->aeb_pub_->publish(aeb_msg);
            return;
        }

        float closing_speed = -relative_speed;
        float time_to_collision = distance / closing_speed;


        if (time_to_collision <= this->ttc_threshold_) {
            RCLCPP_ERROR(this->get_logger(), "Collision imminent, activating brakes!");
            aeb_msg.data = true;
        }
        this->aeb_pub_->publish(aeb_msg);
    };

};

int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    auto aeb_node = std::make_shared<AebNode>();
    rclcpp::spin(aeb_node);
    rclcpp::shutdown();
    return 0;
}
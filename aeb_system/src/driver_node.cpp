#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/vector3.hpp"
#include "std_msgs/msg/bool.hpp"
#include "car.hpp"

using namespace std::chrono_literals;

class DriverNode : public rclcpp::Node {
private:
    Car ego_car;

    bool is_braking = false;
    float delta_t_;
    float brake_deceleration_;

    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<geometry_msgs::msg::Vector3>::SharedPtr car_pub_;
    rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr aeb_sub_;

public:
    DriverNode() : Node("driver_node"),
    ego_car(0.0, 0.0, 0.0, 0.0, 0.0) {

        this->declare_parameter<double>("initial_pos_car", 0.0);
        this->declare_parameter<double>("initial_spe_car", 30.0);
        this->declare_parameter<double>("deceleration", -6.5);
        this->declare_parameter<double>("timer_period", 0.1);
        this->declare_parameter<double>("length", 4.0);

        this->delta_t_ = this->get_parameter("timer_period").as_double();
        this->brake_deceleration_ = this->get_parameter("deceleration").as_double();

        ego_car.pos = this->get_parameter("initial_pos_car").as_double();
        ego_car.speed = this->get_parameter("initial_spe_car").as_double();
        ego_car.length = this->get_parameter("length").as_double();

        timer_ = this->create_wall_timer(
            std::chrono::duration<double>(delta_t_),
            std::bind(&DriverNode::timerCallback, this)
        );

        car_pub_ = this->create_publisher<geometry_msgs::msg::Vector3>("car_data", 10);

        aeb_sub_ = this->create_subscription<std_msgs::msg::Bool>(
            "aeb_signal", 10,
            std::bind(&DriverNode::aebCallback, this, std::placeholders::_1)
        );
    }

    void aebCallback(const std_msgs::msg::Bool::SharedPtr msg) {
        if (msg->data && !this->is_braking) {
            this->is_braking = true;
            ego_car.applyBrakeForce(brake_deceleration_);
            RCLCPP_INFO(this->get_logger(), "AEB activated! Braking applied.");
        } else if (!msg->data && this->is_braking) {
            this->is_braking = false;
            ego_car.releaseBrakeForce();
            RCLCPP_INFO(this->get_logger(), "AEB deactivated! Braking released.");
        }
    }

    void timerCallback() {
        ego_car.updatePhysics(delta_t_);

        geometry_msgs::msg::Vector3 msg;
        msg.x = ego_car.pos;
        msg.y = ego_car.speed;
        msg.z = 0.0;
        car_pub_->publish(msg);
    }

};

int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<DriverNode>());
    rclcpp::shutdown();
    return 0;
}
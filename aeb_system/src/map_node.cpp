#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/vector3.hpp"
#include "car.hpp"
#include <vector>

using namespace std::chrono_literals;

class MapNode : public rclcpp::Node {
private:
    std::vector<Car> npc_cars;
    float delta_t_;

    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<geometry_msgs::msg::Vector3>::SharedPtr map_pub_;

public:
    MapNode() : Node("map_node") {
        this->declare_parameter<double>("initial_pos_obs", 100.0);
        this->declare_parameter<double>("initial_spe_obs", -5.0);
        this->declare_parameter<double>("npc_length", 4.0);
        this->declare_parameter<double>("timer_period", 0.1);

        float pos = this->get_parameter("initial_pos_obs").as_double();
        float speed = this->get_parameter("initial_spe_obs").as_double();
        float len = this->get_parameter("npc_length").as_double();
        this->delta_t_ = this->get_parameter("timer_period").as_double();

        npc_cars.emplace_back(pos, speed, len, 2.0, 1.5);

        map_pub_ = this->create_publisher<geometry_msgs::msg::Vector3>("map_data", 10);

        timer_ = this->create_wall_timer(
            std::chrono::duration<double>(delta_t_),
            std::bind(&MapNode::timerCallback, this)
        );

        RCLCPP_INFO(this->get_logger(), "Map Node has been started.");
    }

    void timerCallback() {
        for (auto& car : npc_cars) {
            car.updatePhysics(delta_t_);
        }

        if(!npc_cars.empty())
        {
            geometry_msgs::msg::Vector3 msg;
            msg.x = npc_cars[0].pos;
            msg.y = npc_cars[0].speed;
            msg.z = 0.0;

            map_pub_->publish(msg);
        }
    }
};

int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    auto map_node = std::make_shared<MapNode>();
    rclcpp::spin(map_node);
    rclcpp::shutdown();
    return 0;
}
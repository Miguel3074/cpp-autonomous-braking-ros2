#include "rclcpp/rclcpp.hpp"
#include "car_node.cpp"

class MapNode : public rclcpp::Node {
private:
    rclcpp::Publisher<geometry_msgs::msg::Vector3>::SharedPtr map_pub_;

public:
    MapNode(const std::string& node_name = "map_node") : Node(node_name) {
        RCLCPP_INFO(this->get_logger(), "Map Node has been started.");
    }
};
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float32.hpp"
#include <chrono>

using namespace std::chrono_literals;

class RadarNode : public rclcpp::Node{
private:
    float pos_car;
    float pos_obs;
    float spe_car;
    float spe_obs;

    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr radar_pub_;

public:
    RadarNode(float initial_pos_obs, float initial_pos_car, float initial_spe_obs, float initial_spe_car, float timer_period): Node("radar_node"){
        this->pos_car = initial_pos_car;
        this->pos_obs = initial_pos_obs;
        this->spe_car = initial_spe_car;
        this->spe_obs = initial_spe_obs;

        timer_= this->create_wall_timer(
            std::chrono::duration<double>(timer_period),
            std::bind(&RadarNode::updateRadarData, this)
        );
        this->radar_pub_ = this->create_publisher<std_msgs::msg::Float32>("radar_data", 10);
    };

    void setCarSpeed(float speed){
        this->spe_car = speed;
    };

    float getCarSpeed(){
        return this->spe_car;
    };

    void setObsSpeed(float speed){
        this->spe_obs = speed;
    };

    float getObsSpeed(){
        return this->spe_obs;
    };

    void updateRadarData(){
        float delta_t = 0.1;
        this->pos_car = this->pos_car + (this->spe_car * delta_t);
        this->pos_obs = this->pos_obs + (this->spe_obs * delta_t);

        float relative_distance = this->pos_obs - this->pos_car;
        RCLCPP_INFO(this->get_logger(), "Car Position: %.2f, Obstacle Position: %.2f, Relative Distance: %.2f", this->pos_car, this->pos_obs, relative_distance);
        auto message = std_msgs::msg::Float32();
        message.data = relative_distance;
        this->radar_pub_->publish(message);
    };

};

int main(int argc, char * argv[]){
    rclcpp::init(argc, argv);
    auto radar_node = std::make_shared<RadarNode>(100.0, 0.0, -5.0, 10.0, 0.1);
    rclcpp::spin(radar_node);
    rclcpp::shutdown();
    return 0;
}

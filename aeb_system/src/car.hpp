#include <algorithm>

class Car{
public:
    float pos;
    float speed;
    float acceleration;

    float length;
    float width;
    float height;

    Car(float p, float s, float l, float w, float h)
        : pos(p), speed(s), acceleration(0.0f), length(l), width(w), height(h) {

    }

    void updatePhysics(float delta_t) {
        this->speed = std::max(0.0f, this->speed + (this->acceleration * delta_t));

        if (this->speed <= 0.0f && this->acceleration < 0.0f) {
            this->speed = 0.0f;
            this->acceleration = 0.0f;
        }

        this->pos += (this->speed * delta_t);
    }

    void applyBrakeForce(float deceleration_rate) {
        this->acceleration = deceleration_rate;
    }

    void releaseBrakeForce() {
        if (this->acceleration < 0.0f) {
            this->acceleration = 0.0f;
        }
    }
};
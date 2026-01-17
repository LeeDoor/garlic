#pragma once
#include "generic_types.hpp"

class Car : public sf::Drawable {
    static constexpr Point CAR_SIZE { 100, 50 };
    static constexpr float ROTATION_DEG_PER_MS = 0.1;
    static constexpr float SPEED_PX_PER_MS = 0.2;
    static constexpr float ROOLING_SPEED_PER_MS = 0.001f;
public:
    Car(Point start_position) : 
        hitbox_{Rectangle::from_position_and_size(start_position, CAR_SIZE)}, 
        direction_{sf::degrees(0)}, 
        wheel_rotation_{0},
        speed_{SPEED_PX_PER_MS} 
    {
        hitbox_.setPosition(start_position);
        hitbox_.setOrigin(hitbox_.getSize() / 2.0f);
    }
    
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        target.draw(hitbox_, states);
    }
    void drive_tick(Time delta_time) {
        direction_ += sf::degrees(wheel_rotation_ * ROTATION_DEG_PER_MS * 0.001f * delta_time.asMicroseconds());
        hitbox_.setRotation(direction_);
        Point normal_shift = get_normal_shift();
        hitbox_.move(normal_shift * speed_ * static_cast<float>(delta_time.asMicroseconds()) * 0.001f);
    }
    void roolie(Time delta_time, int rooling_dir) {
        if(rooling_dir != 0 && std::abs(rooling_dir) != 1)
            rooling_dir /= std::abs(rooling_dir);
        if(rooling_dir == 0) {
            if(wheel_rotation_ != 0) {
                wheel_rotation_ += wheel_rotation_ / -std::abs(wheel_rotation_) * ROOLING_SPEED_PER_MS * delta_time.asMicroseconds() * 0.001f;
            }
        } else if(rooling_dir == -1) {
            if(wheel_rotation_ > rooling_dir) {
                wheel_rotation_ -= ROOLING_SPEED_PER_MS * delta_time.asMicroseconds() * 0.001f;
            }
        } else if(rooling_dir == 1) {
            if(wheel_rotation_ < rooling_dir) {
                wheel_rotation_ += ROOLING_SPEED_PER_MS * delta_time.asMicroseconds() * 0.001f;
            }
        }
    }
private:
    Point get_normal_shift() const {
        return {
            static_cast<float>(cos(direction_.asRadians())), 
            static_cast<float>(sin(direction_.asRadians()))
        };
    }

    Rectangle hitbox_;
    Direction direction_;
    float wheel_rotation_; // [-1.0; 1.0]
    PxPerMs speed_;
};

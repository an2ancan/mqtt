#include "bicycle_model.hpp"
#include <ncurses.h>

constexpr auto DELTA_V = 0.5;
constexpr auto DELTA_PHI = 0.174;  //10 degree

namespace model{

void BicycleModel::increase_speed() { 
    std::lock_guard<std::mutex> lk(mtx_ctrl_);
    control_.speed += DELTA_V;
    std::min(control_.speed, max_speed_);
}
void BicycleModel::decrease_speed() { 
    std::lock_guard<std::mutex> lk(mtx_ctrl_);
    control_.speed -= DELTA_V;
    std::max(control_.speed, -max_speed_);
}

void BicycleModel::increase_steering() { 
    std::lock_guard<std::mutex> lk(mtx_ctrl_);
    control_.phi += DELTA_PHI;
    std::min(control_.speed, max_steering_);
}

void BicycleModel::decrease_steering() { 
    std::lock_guard<std::mutex> lk(mtx_ctrl_);
    control_.phi -= DELTA_PHI;
    std::max(control_.speed, -max_steering_);
}

State BicycleModel::update(double delta_t){
    auto ctrl = get_cntrl_copy();
    cur_state_.x_dot = control_.speed * cos(ctrl.phi + cur_state_.theta);
    cur_state_.y_dot = control_.speed * sin(ctrl.phi + cur_state_.theta);
    auto theta_dot = control_.speed * sin(ctrl.phi) / length_;

    cur_state_.x += cur_state_.x_dot * delta_t;
    cur_state_.y += cur_state_.y_dot * delta_t;
    cur_state_.theta += theta_dot * delta_t;
    return cur_state_;
}
}//model
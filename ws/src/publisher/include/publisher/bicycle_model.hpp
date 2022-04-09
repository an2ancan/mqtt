#pragma once

#include <math.h>
#include <mutex>

//the kinematic model of the forklift 
//using bycicle model from https://dingyan89.medium.com/simple-understanding-of-kinematic-bicycle-model-81cac6420357

namespace model{

struct State {
    double x = 0.0;
    double y = 0.0;
    double z = 1.0;
    double x_dot = 0.0;
    double y_dot = 0.0;
    double theta = 0.0;
};

struct Control {
    double speed;
    double phi;  //streering
};

class BicycleModel {
public:
    State update(double delta_t);
    void increase_speed();
    void decrease_speed();
    void increase_steering();
    void decrease_steering();
private:
    Control get_cntrl_copy() const {
        std::lock_guard<std::mutex> lk(mtx_ctrl_);
        return control_;
    }
private:
    mutable std::mutex mtx_ctrl_;
    mutable std::mutex mtx_st_;
    State cur_state_ = {};
    Control control_ = {};
    double length_ = 2.0; //length of base
    double max_speed_ = 10.0;
    double max_steering_ = M_PI_4;
};
}//model
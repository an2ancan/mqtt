#pragma once 

#include "bicycle_model.hpp"
#include "action_listner.hpp"
#include "callback.hpp"
#include "mqtt/async_client.h"
#include <ncurses.h>
#include <fstream>

namespace publisher {

enum class ModelAction {
    SPEEED_UP = 0,
    SPEED_DOWN,
    T0_THE_LEFT,
    TO_THE_RIGHT,
    NO_ACTION
};

class Publisher {
public:
    Publisher();
    ~Publisher();
    void publish();
    void propogate_model();
    void update_control(const ModelAction action);
private:
    model::BicycleModel model_ = {};
    DeliveryActionListner listener_ = {};
    Callback callback_ = {};
    model::State last_state_ = {};
    mqtt::async_client client_;
    std::chrono::steady_clock::time_point start_;
    std::chrono::steady_clock::time_point prev_time_point_;
    std::ofstream file_ {"/home/ws/position.csv"};
};

} // publisher

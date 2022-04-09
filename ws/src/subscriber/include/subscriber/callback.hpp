#pragma once

#include "action_listner.hpp"
#include "diff_accumulator.hpp"
#include <fstream>

/**
 * Local callback & listener class for use with the client connection.
 * This is primarily intended to receive messages, but it will also monitor
 * the connection to the broker. If the connection is lost, it will attempt
 * to restore the connection and re-subscribe to the topic.
 */

namespace subscriber {

class Callback : public virtual mqtt::callback,
					public virtual mqtt::iaction_listener
{
public:
    struct config_s {
        std::string address;
        std::string client_id;
        std::string topic;
        int qos;     
    };
    Callback(mqtt::async_client& cli, mqtt::connect_options& connOpts, 
        const config_s& callback_config )
            :   nretry_(0), 
                cli_(cli), 
                connOpts_(connOpts), 
                listener_("Subscription"),
                config_(callback_config){ file_ << "time,accum,div\n";}
private:
    void reconnect();
    void on_failure(const mqtt::token& tok) override;
    void on_success(const mqtt::token& tok) override;
    void connected(const std::string& cause) override;
    void connection_lost(const std::string& cause) override;
    void message_arrived(mqtt::const_message_ptr msg) override;
    void delivery_complete(mqtt::delivery_token_ptr token) override;
private:
	// Counter for the number of connection retries
	int nretry_;
	// The MQTT client
	mqtt::async_client& cli_;
	// Options to use if we need to reconnect
	mqtt::connect_options& connOpts_;
	// An action listener to display the result of actions.
	ActionListner listener_;
    config_s config_ ;
    DistanceAccumulator accum_ = {};
    Derivative deriv_ = {};
    std::ofstream file_ {"/home/ws/accum_div.csv"};
};


} // sub_callback

#include "callback.hpp"
#include "tag_position.hpp"

constexpr int	N_RETRY_ATTEMPTS = 5;

namespace subscriber {

void Callback::reconnect() {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    try {
        cli_.connect(connOpts_, nullptr, *this);
    }
    catch (const mqtt::exception& exc) {
        std::cerr << "Error: " << exc.what() << std::endl;
        exit(1);
    }
}

// Re-connection failure
void Callback::on_failure(const mqtt::token& tok) {
    std::cout << "Connection attempt failed" << std::endl;
    if (++nretry_ > N_RETRY_ATTEMPTS)
        exit(1);
    reconnect();
}

// (Re)connection success
void Callback::on_success(const mqtt::token& tok) {}

// (Re)connection success
void Callback::connected(const std::string& cause) {
    std::cout << "\nConnection success" << std::endl;
    std::cout << "\nSubscribing to topic '" << config_.topic << "'\n"
        << "\tfor client " << config_.client_id
        << " using QoS" << config_.qos << "\n"
        << "\nPress Q<Enter> to quit\n" << std::endl;

    cli_.subscribe(config_.topic, config_.qos, nullptr, listener_);
}

// Callback for when the connection is lost.
// This will initiate the attempt to manually reconnect.
void Callback::connection_lost(const std::string& cause) {
    std::cout << "\nConnection lost" << std::endl;
    if (!cause.empty())
        std::cout << "\tcause: " << cause << std::endl;

    std::cout << "Reconnecting..." << std::endl;
    nretry_ = 0;
    reconnect();
}

// Callback for when a message arrives.
void Callback::message_arrived(mqtt::const_message_ptr msg) {
    std::cout << "\ttopic: '" << msg->get_topic() << "'" << std::endl;
    if (msg->get_topic() == position_topic::TOPIC_NAME){
        //belive there might be a better way, but havent'f found it
        const position_topic::Position* pos = 
            reinterpret_cast<const position_topic::Position*>(msg->get_payload().c_str());
        std::cout << *pos << std::endl;
        Val3D space_pos {pos->x, pos->y, pos->z}; 
        const double acum_val = accum_.accumulate(space_pos);
        const double div = deriv_.derivate(space_pos, pos->time_stamp);
        std::cout << "acum: " << acum_val << " div: " << div << "\n";
        file_ << pos->time_stamp << "," << acum_val << "," << div << "\n";
    } else {
        std::cout << "\tpayload: '" << msg->to_string() << "'\n" << std::endl;
    }
}

void Callback::delivery_complete(mqtt::delivery_token_ptr token) {}
} //sub_callback
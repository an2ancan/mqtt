#include "publisher.hpp"
#include "tag_position.hpp"
#include "sstream"

constexpr auto SERVER_ADDRESS	= "tcp://localhost:1883"; 
constexpr auto TOPIC  = "tag_position";
constexpr auto  CLIENT_ID = "tag_position_publisher";
constexpr auto PERSIST_DIR = "./persist";
constexpr auto  QOS = 1;
constexpr auto TAG_ID = 777;

using namespace std;

namespace publisher {
Publisher::Publisher():
    client_(SERVER_ADDRESS, CLIENT_ID, PERSIST_DIR),
    start_(chrono::steady_clock::now()),
    prev_time_point_(chrono::steady_clock::now())
{
    file_ << "time,x,y,z\n";
    printw("Initializing for server '%s'...\n", SERVER_ADDRESS);
    refresh();

    Callback cb;
    client_.set_callback(cb);

    auto connOpts = mqtt::connect_options_builder()
        .clean_session()
        // .will(mqtt::message(TOPIC, LWT_PAYLOAD, QOS))
        .finalize();

    printw("  ...OK\n");

    printw("\nConnecting...\n");
    
    mqtt::token_ptr conntok = client_.connect(connOpts);
    printw("Waiting for the connection...\n");
    conntok->wait();
    printw("  ...OK\n");
    refresh();
}

Publisher::~Publisher(){
    try {
        // Double check that there are no pending tokens
        auto toks = client_.get_pending_delivery_tokens();
        if (!toks.empty()){
            printw("Error: There are pending delivery tokens!\n");
            refresh();
        }
        
        // Disconnect
        printw("\nDisconnecting...\n");
        refresh();
        client_.disconnect()->wait();
        printw("  ...OK\n");
        refresh();
    } catch (const mqtt::exception& exc) {
        cerr << exc.what() << endl;
    } catch( ... ) { }
}

void Publisher::publish(){
    double time_stamp = 
        chrono::duration<double>(chrono::steady_clock::now() - start_).count();
    auto position = make_shared<position_topic::Position>(
        time_stamp,
        TAG_ID,
        last_state_.x,
        last_state_.y,
        last_state_.z
    );

    //must be a better way
    char* payload_c = reinterpret_cast<char*>(position.get());
    mqtt::message_ptr pubmsg  = mqtt::make_message(TOPIC, payload_c, sizeof *position );
    client_.publish(pubmsg, nullptr, listener_);

    //write data to the csv_file
    file_ << position->time_stamp << "," 
        << position->x << "," << position->y << "," << position->z <<"\n";
    while (!listener_.is_done()) {
        this_thread::sleep_for(std::chrono::microseconds(100));
    }
}

void Publisher::update_control(const ModelAction action){
    switch (action){
    case ModelAction::SPEEED_UP:
        model_.increase_speed();
        break;
    case ModelAction::SPEED_DOWN:
        model_.decrease_speed();
        break;
    case ModelAction::T0_THE_LEFT:
        model_.increase_steering();
        break;
    case ModelAction::TO_THE_RIGHT:
        model_.decrease_steering();
        break;
    case ModelAction::NO_ACTION:
        break;
    default:
        std::cerr << "unkwon command\n" ;//+++ exception?
    }
}

void Publisher::propogate_model(){
    auto delta_t = chrono::duration<double>(chrono::steady_clock::now() - prev_time_point_).count();
    prev_time_point_ = chrono::steady_clock::now();
    last_state_ = std::move(model_.update(delta_t));
}

} // publisher
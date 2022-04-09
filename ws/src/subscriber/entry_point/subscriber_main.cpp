
#include <thread>
#include <chrono>

#include "callback.hpp"

constexpr auto SERVER_ADDRESS	= "tcp://localhost:1883"; 
constexpr auto TOPIC  = "tag_position";
constexpr auto  CLIENT_ID = "tag_position_subscriber";
constexpr int	QOS = 1;

int main()
{
	mqtt::async_client cli(SERVER_ADDRESS, CLIENT_ID);

	mqtt::connect_options connOpts;
	connOpts.set_clean_session(false);

	// Install the callback(s) before connecting.
	subscriber::Callback cb(cli, connOpts, {SERVER_ADDRESS, CLIENT_ID, TOPIC, QOS});
	cli.set_callback(cb);

	// Start the connection.
	// When completed, the callback will subscribe to topic.

	try {
		std::cout << "Connecting to the MQTT server..." << std::flush;
		cli.connect(connOpts, nullptr, cb);
	}
	catch (const mqtt::exception& exc) {
		std::cerr << "\nERROR: Unable to connect to MQTT server: '"
			<< SERVER_ADDRESS << "'" << exc << std::endl;
		return 1;
	}

	// Just block till user tells us to quit.
	while (std::tolower(std::cin.get()) != 'q');

	// Disconnect
	try {
		std::cout << "\nDisconnecting from the MQTT server..." << std::flush;
		cli.disconnect()->wait();
		std::cout << "OK" << std::endl;
	}
	catch (const mqtt::exception& exc) {
		std::cerr << exc << std::endl;
		return 1;
	}

 	return 0;
}


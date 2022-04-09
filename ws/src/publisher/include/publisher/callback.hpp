#pragma once

#include <iostream>
#include <cstdlib>
#include <string>
#include <ncurses.h>

#include "mqtt/async_client.h"

namespace publisher {
/**
 * A callback class for use with the main MQTT client.
 */
class Callback : public virtual mqtt::callback
{
public:
	void connection_lost(const std::string& cause) override {
		printw("\nConnection lost\n");
		refresh();
		if (!cause.empty()) {
			printw("\tcause: %s\n");
			refresh();
		}
	}

	void delivery_complete(mqtt::delivery_token_ptr tok) override {
		printw("\tDelivery complete for token: %d\n",
			(tok ? tok->get_message_id() : -1));
		refresh();
	}
};
} // pub_callback
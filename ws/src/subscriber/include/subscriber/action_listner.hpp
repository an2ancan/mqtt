#pragma once

#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <cctype>
#include "mqtt/async_client.h"

// Callbacks for the success or failures of requested actions.
// log the results to the console.

namespace subscriber{
class ActionListner : public virtual mqtt::iaction_listener
{
	std::string name_;

	void on_failure(const mqtt::token& tok) override {
		std::cout << name_ << " failure";
		if (tok.get_message_id() != 0)
			std::cout << " for token: [" << tok.get_message_id() << "]" << std::endl;
		std::cout << std::endl;
	}

	void on_success(const mqtt::token& tok) override {
		std::cout << name_ << " success";
		if (tok.get_message_id() != 0)
			std::cout << " for token: [" << tok.get_message_id() << "]" << std::endl;
		auto top = tok.get_topics();
		if (top && !top->empty())
			std::cout << "\ttoken topic: '" << (*top)[0] << "', ..." << std::endl;
		std::cout << std::endl;
	}

public:
	ActionListner(const std::string& name) : name_(name) {}
};
}//sub_action_listner
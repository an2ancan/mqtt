#pragma once

#include <iostream>
#include <cstdlib>
#include <string>
#include <thread>
#include <atomic>
#include "mqtt/async_client.h"

namespace publisher {
/**
 * A base action listener.
 */
class ActionListner : public virtual mqtt::iaction_listener
{
protected:
	void on_failure(const mqtt::token& tok) override {
		// std::cout << "\tListener failure for token: "
		// 	<< tok.get_message_id() << std::endl;
	}

	void on_success(const mqtt::token& tok) override {
		// std::cout << "\tListener success for token: "
		// 	<< tok.get_message_id() << std::endl;
	}
};

/////////////////////////////////////////////////////////////////////////////

/**
 * A derived action listener for publish events.
 */
class DeliveryActionListner : public ActionListner
{
	std::atomic<bool> done_;

	void on_failure(const mqtt::token& tok) override {
		ActionListner::on_failure(tok);
		done_ = true;
	}

	void on_success(const mqtt::token& tok) override {
		ActionListner::on_success(tok);
		done_ = true;
	}

public:
	DeliveryActionListner() : done_(false) {}
	bool is_done() const { return done_; }
};
}//pub_listner
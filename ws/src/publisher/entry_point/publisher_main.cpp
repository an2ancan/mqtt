#include <cstring>
#include <map>
#include "mqtt/async_client.h"

#include "action_listner.hpp"
#include "publisher.hpp"
#include <ncurses.h>
#include <thread>
#include <chrono>

using namespace std;

constexpr auto PERIOD = 100; //ms
const auto PERIOD_MS {chrono::milliseconds(PERIOD)};

map<char, publisher::ModelAction> actions_keys = {
	{'w', publisher::ModelAction::SPEEED_UP}, 
	{'s', publisher::ModelAction::SPEED_DOWN}, 
	{'d', publisher::ModelAction::TO_THE_RIGHT}, 
	{'a', publisher::ModelAction::T0_THE_LEFT} 
};

std::atomic<bool> do_terminate{false};

void scan_keyboard_and_update_control(shared_ptr<publisher::Publisher> generator){
	//scan terminal and update control accrodingly actions_keys
	char c = ' ';
	while (c != 'q'){
		c = getch();
		auto it = actions_keys.find(c);
		if (it!=actions_keys.end()) {
			generator->update_control(it->second);
		}
	}
	//q is pressed, can stop the work now
	do_terminate = true;
}

void generate_pos(shared_ptr<publisher::Publisher> generator) {
	while(!do_terminate){
		generator->propogate_model();
		generator->publish();
		this_thread::sleep_for(PERIOD_MS);
	}
}

int main()
{
	initscr();
	auto generator = make_shared<publisher::Publisher>();

	thread t1 {scan_keyboard_and_update_control, generator};
	thread t2 {generate_pos, generator};

	t1.join();
	t2.join();

	endwin();
 	return 0;
}

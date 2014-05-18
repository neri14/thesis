#ifndef ALGORITHM_OBJECT_H
#define ALGORITHM_OBJECT_H

#include <boost/weak_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <string>
#include <set>
#include <map>
#include <queue>
#include "controller_def.h"

namespace controller {

class algorithm_object
{
public:
	algorithm_object(std::string name_);

	void notif_time_tick(int tick);
	int set_state(bool allow, int token);
	//returns in which time tick after setting it will be new state

	bool has_state_changed();
	const std::string& get_name();
	EActuatorState get_state();

	void add_dependant(boost::weak_ptr<algorithm_object> obj);
	void add_collision(boost::weak_ptr<algorithm_object> obj, int cooldown);

	int get_current_state_timer();
	bool is_during_state_change();

private:
	const std::vector<EActuatorState>& get_transition(EActuatorState from, EActuatorState to);

	std::string name;
	EActuatorState current_state;
	EActuatorState going_to_state;
	std::queue<EActuatorState> state_queue; // one state shows one second
	int current_state_timer;
	bool state_changed;
	int last_tick;

	std::set<boost::weak_ptr<algorithm_object> > dependant;
	std::map<boost::weak_ptr<algorithm_object>, int> collision;
};
typedef boost::shared_ptr<algorithm_object> algorithm_object_handle;

}

#endif /* ALGORITHM_OBJECT_H */
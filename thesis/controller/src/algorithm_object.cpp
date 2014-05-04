#include "algorithm_object.h"

#include <boost/foreach.hpp>
#include <boost/assign.hpp>

namespace controller {
namespace constant {
	const int yellow_time(3);
	const int red_yellow_time(1);

	const std::vector<EActuatorState> transition_red_to_green = boost::assign::list_of
		(EActuatorState_RedYellow)
		(EActuatorState_Green);
	const std::vector<EActuatorState> transition_green_to_red = boost::assign::list_of
		(EActuatorState_Yellow)
		(EActuatorState_Yellow)
		(EActuatorState_Yellow)
		(EActuatorState_Red);
	const std::vector<EActuatorState> transition_off_to_green = boost::assign::list_of
		(EActuatorState_Red)
		(EActuatorState_RedYellow)
		(EActuatorState_Green);
	const std::vector<EActuatorState> transition_off_to_red = boost::assign::list_of
		(EActuatorState_Red);
}

algorithm_object::algorithm_object(std::string name_) :
	name(name_),
	current_state(EActuatorState_Off),
	going_to_state(EActuatorState_Off),
	current_state_timer(0),
	state_changed(false),
	last_tick(-1)
{}

void algorithm_object::notif_time_tick(int tick)
{
	if (last_tick < tick) {
		last_tick = tick;
		state_changed = false;
		if (!state_queue.empty()) {
			EActuatorState new_state = state_queue.front();
			if (new_state != current_state) {
				current_state = new_state;
				state_changed = true;
			}
			state_queue.pop();
		}

		if (state_changed) {
			current_state_timer = 1;
		} else {
			++current_state_timer;
		}
	}
}

int algorithm_object::set_state(bool allow, int token)
{
	static int last_token = -1;
	if (token <= last_token) {
		return 0;
	}
	last_token = token;

	if (allow && going_to_state != EActuatorState_Green) {
		std::vector<EActuatorState> transition =
			get_transition(going_to_state, EActuatorState_Green);
		int delay = 0;

		BOOST_FOREACH(boost::weak_ptr<algorithm_object> ob, dependant) {
			ob.lock()->set_state(true, token);
		}

		typedef std::pair<boost::weak_ptr<algorithm_object>, int> collision_type;
		BOOST_FOREACH(collision_type ob, collision) {
			delay = std::max(ob.first.lock()->set_state(false, token) + ob.second, delay);
		}

		if (going_to_state == EActuatorState_Red) {
			for (int i=0; i<delay; ++i) {
				state_queue.push(going_to_state);
			}
		}
		BOOST_FOREACH(EActuatorState st, transition) {
			state_queue.push(st);
		}
		going_to_state = EActuatorState_Green;
	} else if (!allow && going_to_state != EActuatorState_Red) {
		std::vector<EActuatorState> transition =
			get_transition(going_to_state, EActuatorState_Red);

		BOOST_FOREACH(EActuatorState st, transition) {
			state_queue.push(st);
		}
		going_to_state = EActuatorState_Red;
	}

	return state_queue.size();
}

bool algorithm_object::has_state_changed()
{
	return state_changed;
}

const std::string& algorithm_object::get_name()
{
	return name;
}

EActuatorState algorithm_object::get_state()
{
	return current_state;
}

void algorithm_object::add_dependant(boost::weak_ptr<algorithm_object> obj)
{
	dependant.insert(obj);
}

void algorithm_object::add_collision(boost::weak_ptr<algorithm_object> obj, int cooldown)
{
	collision.insert(std::make_pair(obj, cooldown));
}

const std::vector<EActuatorState>& algorithm_object::get_transition(
	EActuatorState from, EActuatorState to)
{
	if (from == EActuatorState_Red && to == EActuatorState_Green) {
		return constant::transition_red_to_green;
	} else if (from == EActuatorState_Green && to == EActuatorState_Red) {
		return constant::transition_green_to_red;
	} else if (from == EActuatorState_Off && to == EActuatorState_Green) {
		return constant::transition_off_to_green;
	} else {
		return constant::transition_off_to_red;
	}
}

}
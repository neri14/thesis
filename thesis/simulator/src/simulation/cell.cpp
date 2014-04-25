#include "cell.h"

namespace simulator {
namespace simulation {
namespace constant {
std::map<EExitState, bool> exit_allowed = boost::assign::map_list_of
	(EExitState_Off, true) (EExitState_Red, false) (EExitState_RedYellow, true)
	(EExitState_Green, true) (EExitState_Yellow, false);
} // namespace constant

cell::cell(int priority_entrance) :
	priority_entrance_number(priority_entrance),
	vehicle_counter(0)
{}

bool cell::is_exit_allowed(int ex)
{
	boost::mutex::scoped_lock lock(mtx_exit_states);
	std::map<int, boost::weak_ptr<cell> >::iterator it = next.find(ex);
	if (it == next.end()) {
		return false;
	}
	boost::weak_ptr<cell> c = it->second;

	BOOST_ASSERT(exit_states.end() != exit_states.find(c));
	return constant::exit_allowed.find(exit_states.find(c)->second)->second;
}

bool cell::set_exit_state(int ex, EExitState state)
{
	boost::mutex::scoped_lock lock(mtx_exit_states);
	std::map<int, boost::weak_ptr<cell> >::iterator it = next.find(ex);
	if (it == next.end()) {
		return false;
	}
	boost::weak_ptr<cell> c = it->second;

	BOOST_ASSERT(exit_states.end() != exit_states.find(c));
	exit_states[c] = state;
	return true;
}

EExitState cell::get_exit_state(int ex)
{
	boost::mutex::scoped_lock lock(mtx_exit_states);
	std::map<int, boost::weak_ptr<cell> >::iterator it = next.find(ex);
	if (it == next.end()) {
		return EExitState_Off;
	}
	boost::weak_ptr<cell> c = it->second;

	BOOST_ASSERT(exit_states.end() != exit_states.find(c));
	return exit_states.find(c)->second;
}

void cell::increment_vehicle_counter()
{
	++vehicle_counter;
}

int cell::get_vehicle_counter_value()
{
	return vehicle_counter;
}

bool cell::is_occupied()
{
	return current_vehicle;
}

void cell::add_prev(int entrance, boost::weak_ptr<cell> c)
{
	prev[entrance] = c;
}

void cell::add_next(int ex, boost::weak_ptr<cell> c)
{
	next[ex] = c;
	exit_states[c] = EExitState_Off;
}

boost::weak_ptr<cell> cell::get_prev(int ent)
{
	if (prev.find(ent) == prev.end()) {
		return boost::weak_ptr<cell>();
	} else {
		return prev[ent];
	}
}

boost::weak_ptr<cell> cell::get_next(int ex)
{
	if (next.find(ex) == next.end()) {
		return boost::weak_ptr<cell>();
	} else {
		return next[ex];
	}
}

} // namespace simulation
} // namespace simulator

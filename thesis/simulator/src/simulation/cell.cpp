#include "cell.h"

namespace simulator {
namespace simulation {
namespace constant {
std::map<EExitState, bool> exit_allowed = boost::assign::map_list_of
	(EExitState_Off, true) (EExitState_Red, false) (EExitState_RedYellow, true)
	(EExitState_Green, true) (EExitState_Yellow, false);
} // namespace constant

cell::cell() :
	priority_entrance_number(0),
	occupied(false)
{}

void cell::add_prev(int entrance, boost::weak_ptr<cell> c)
{
	prev[entrance] = c;
}

void cell::add_next(int ex, boost::weak_ptr<cell> c)
{
	next[ex] = c;
	exit_states[c] = EExitState_Off;
}

bool cell::is_exit_allowed(int ex)
{
	std::map<int, boost::weak_ptr<cell> >::iterator it = next.find(ex);
	if (it == next.end()) {
		return false;
	}
	boost::weak_ptr<cell> c = it->second;

	BOOST_ASSERT(exit_states.end() != exit_states.find(c));
	return constant::exit_allowed.find(exit_states.find(c)->second)->second;
}

} // namespace simulation
} // namespace simulator

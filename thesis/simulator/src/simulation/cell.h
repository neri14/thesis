#ifndef CELL_H
#define CELL_H

#include <boost/assign.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <map>

namespace simulator {
namespace simulation {

enum EExitState {
	EExitState_Off,
	EExitState_Red,
	EExitState_RedYellow,
	EExitState_Green,
	EExitState_Yellow
};

struct cell
{
	cell();

	std::map<int, boost::weak_ptr<cell> > prev;
	std::map<int, boost::weak_ptr<cell> > next;

	std::map<boost::weak_ptr<cell>, EExitState> exit_states;

	int priority_entrance_number;

	bool occupied;//FIXME replace bool with ptr to vehicle
};
typedef boost::shared_ptr<cell> cell_handle;

} // namespace simulation
} // namespace simulator

#endif /* CELL_H */

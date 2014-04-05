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

//namespace constant {
//std::map<EExitState, bool> exit_allowed = boost::assign::map_list_of
//	(EExitState_Off, true) (EExitState_Red, false) (EExitState_RedYellow, true)
//	(EExitState_Green, true) (EExitState_Yellow, false);
//} // namespace constant

struct cell
{
	std::map<int, boost::weak_ptr<cell> > prev;
	std::map<int, boost::weak_ptr<cell> > next;

	std::map<boost::weak_ptr<cell>, EExitState> exit_states;

	int priority_entrance_number;
	boost::weak_ptr<cell> priority_entrance;

	bool occupied;
};
typedef boost::shared_ptr<cell> cell_handle;

} // namespace simulation
} // namespace simulator

#endif /* CELL_H */

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

} // namespace simulation
} // namespace simulator

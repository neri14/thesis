#include "actuator.h"

#include <boost/bind.hpp>

namespace simulator {
namespace simulation {

actuator::actuator(std::string area_name_,
		common::dispatcher::EEventScope area_scope_, std::string actuator_name_,
		cell_handle controlled_cell_, int controlled_exit_) :
	area_name(area_name_),
	area_scope(area_scope_),
	actuator_name(actuator_name_),
	controlled_cell(controlled_cell_),
	controlled_exit(controlled_exit_)
{
	listener = common::dispatcher::get_dispatcher().register_listener(
		common::dispatcher::EEventType_SetActuatorState, area_scope_,
		boost::bind(&actuator::on_event, this, _1));
	//register_listener(type, scope,
	//	boost::bind(&ut_event_dispatcher::on_event, this, _1));
}

void actuator::on_event(common::dispatcher::event_handle ev)
{
	BOOST_ASSERT(common::dispatcher::EEventType_SetActuatorState == ev->get_type());
	BOOST_ASSERT(area_scope == ev->get_scope());

	common::dispatcher::EActuatorState state =
		ev->get_payload<common::dispatcher::set_actuator_state_payload>()->state;

	//FIXME replace with mutex protected set_exit_state
	controlled_cell->exit_states[controlled_cell] = map_state(state);
}

EExitState actuator::map_state(common::dispatcher::EActuatorState state)
{
	static std::map<common::dispatcher::EActuatorState, EExitState> states =
		boost::assign::map_list_of
			(common::dispatcher::EActuatorState_Off, EExitState_Off)
			(common::dispatcher::EActuatorState_Red, EExitState_Red)
			(common::dispatcher::EActuatorState_RedYellow, EExitState_RedYellow)
			(common::dispatcher::EActuatorState_Green, EExitState_Green)
			(common::dispatcher::EActuatorState_Yellow, EExitState_Yellow);

	return states.find(state)->second;
}

}
}
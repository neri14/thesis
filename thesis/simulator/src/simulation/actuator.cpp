#include "actuator.h"

#include <csv_writer.h>

#include <dispatcher/event/payload/actuator_finished_payload.h>
#include <dispatcher/event/payload/time_tick_payload.h>

#include <boost/bind.hpp>

namespace simulator {
namespace simulation {

actuator::actuator(std::string area_name_,
		EEventScope area_scope_, std::string actuator_name_,
		cell_handle controlled_cell_, int controlled_exit_) :
	logger(std::string("actuator_")+actuator_name_),
	area_name(area_name_),
	area_scope(area_scope_),
	actuator_name(actuator_name_),
	controlled_cell(controlled_cell_),
	controlled_exit(controlled_exit_)
{
	set_state_listener = common::dispatcher::get_dispatcher().register_listener(
		EEventType_SetActuatorState, area_scope_,
		boost::bind(&actuator::on_set_state, this, _1));
	time_tick_listener = common::dispatcher::get_dispatcher().register_listener(
		EEventType_TimeTick, EEventScope_General,
		boost::bind(&actuator::on_time_tick, this, _1));

	common::get_csv_writer().add_key(actuator_name);
}

actuator::~actuator()
{
	common::dispatcher::get_dispatcher().unregister_listener(set_state_listener);
	common::dispatcher::get_dispatcher().unregister_listener(time_tick_listener);
}

const std::string& actuator::get_name() const
{
	return actuator_name;
}

void actuator::on_set_state(common::dispatcher::event_handle ev)
{
	BOOST_ASSERT(EEventType_SetActuatorState == ev->get_type());
	BOOST_ASSERT(area_scope == ev->get_scope());
	common::dispatcher::set_actuator_state_payload& payload =
		*ev->get_payload<common::dispatcher::set_actuator_state_payload>();

	if (actuator_name == payload.get_actuator_name()) {
		logger.info()() << "received new state " << payload.state;
		common::dispatcher::EActuatorState state = payload.state;
		pending_state = map_state(state);
	}
}

void actuator::on_time_tick(common::dispatcher::event_handle ev)
{
	BOOST_ASSERT(EEventType_TimeTick == ev->get_type());
	BOOST_ASSERT(EEventScope_General == ev->get_scope());
	int time_tick = ev->get_payload<common::dispatcher::time_tick_payload>()->tick;

	if (pending_state) {
		logger.info()() << "setting new state " << pending_state.get();
		controlled_cell->set_exit_state(controlled_exit, pending_state.get());
		pending_state = boost::none;
	}

	common::get_csv_writer().add_value(time_tick, actuator_name, controlled_cell->get_exit_state(controlled_exit));

	logger.debug()() << "actuator finished";
	common::dispatcher::payload_handle payload(
		new common::dispatcher::actuator_finished_payload(actuator_name, time_tick));
	common::dispatcher::get_dispatcher().dispatch(common::dispatcher::event_handle(
		new common::dispatcher::event(EEventType_ActuatorFinished, EEventScope_Local, payload)));
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
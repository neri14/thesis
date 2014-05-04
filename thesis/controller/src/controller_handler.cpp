#include "controller_handler.h"

#include "algorithm_factory.h"
#include <config/config.h>

#include <dispatcher/event/event.h>
#include <dispatcher/event/payload/queue_sensor_state.h>
#include <dispatcher/event/payload/flow_sensor_state.h>
#include <dispatcher/event/payload/time_tick_payload.h>
#include <dispatcher/event/payload/set_actuator_state_payload.h>

#include <boost/bind.hpp>
#include <boost/assign.hpp>

namespace controller {

controller_handler::controller_handler(controlled_area_data area_) :
	logger("controller_handler"),
	area(area_)
{
	alg = algorithm_factory::create(common::get_config().get<std::string>("algorithm"),
		boost::bind(&controller_handler::set_actuator_state, this, _1, _2));
}

controller_handler::~controller_handler()
{
	stop();
}

void controller_handler::start()
{
	logger.info()() << "starting control";

	queue_listener = common::dispatcher::get_dispatcher().register_listener(
		EEventType_QueueSensorState, static_cast<EEventScope>(area.scope),
		boost::bind(&controller_handler::on_queue_event, this, _1));
	flow_listener = common::dispatcher::get_dispatcher().register_listener(
		EEventType_FlowSensorState, static_cast<EEventScope>(area.scope),
		boost::bind(&controller_handler::on_flow_event, this, _1));
	time_listener = common::dispatcher::get_dispatcher().register_listener(
		EEventType_TimeTick,EEventScope_General,
		boost::bind(&controller_handler::on_time_event, this, _1));
}

void controller_handler::stop()
{
	if (queue_listener) {
		common::dispatcher::get_dispatcher().unregister_listener(queue_listener);
	}
	if (flow_listener) {
		common::dispatcher::get_dispatcher().unregister_listener(flow_listener);
	}
	if (time_listener) {
		common::dispatcher::get_dispatcher().unregister_listener(time_listener);
	}
}

void controller_handler::on_queue_event(common::dispatcher::event_handle ev)
{
	BOOST_ASSERT(EEventType_QueueSensorState == ev->get_type());
	BOOST_ASSERT(static_cast<EEventScope>(area.scope) == ev->get_scope());
	common::dispatcher::queue_sensor_state& payload =
		*ev->get_payload<common::dispatcher::queue_sensor_state>();

	logger.info()() << "[" << payload.time_tick << "] queue " << payload.get_sensor_name() << 
		" : " << payload.queue << " / " << payload.max_queue;
	alg->on_queue_sensor_update(
		payload.get_sensor_name(), payload.time_tick, payload.queue, payload.max_queue);
}

void controller_handler::on_flow_event(common::dispatcher::event_handle ev)
{
	BOOST_ASSERT(EEventType_FlowSensorState == ev->get_type());
	BOOST_ASSERT(static_cast<EEventScope>(area.scope) == ev->get_scope());
	common::dispatcher::flow_sensor_state& payload =
		*ev->get_payload<common::dispatcher::flow_sensor_state>();

	logger.info()() << "[" << payload.time_tick << "] flow " << payload.get_sensor_name() << 
		" : " << payload.flow << " cars/h ";
	alg->on_flow_sensor_update(payload.get_sensor_name(), payload.time_tick, payload.flow);
}

void controller_handler::on_time_event(common::dispatcher::event_handle ev)
{
	BOOST_ASSERT(EEventType_TimeTick == ev->get_type());
	BOOST_ASSERT(EEventScope_General == ev->get_scope());
	int time_tick = ev->get_payload<common::dispatcher::time_tick_payload>()->tick;

	logger.info()() << "[" << time_tick << "] time tick";
	alg->on_time_tick(time_tick);
}

void controller_handler::set_actuator_state(const std::string& name, EActuatorState state)
{
	static std::map<EActuatorState, common::dispatcher::EActuatorState> map = boost::assign::map_list_of
		(EActuatorState_Off, common::dispatcher::EActuatorState_Off)
		(EActuatorState_Red, common::dispatcher::EActuatorState_Red)
		(EActuatorState_RedYellow, common::dispatcher::EActuatorState_RedYellow)
		(EActuatorState_Green, common::dispatcher::EActuatorState_Green)
		(EActuatorState_Yellow, common::dispatcher::EActuatorState_Yellow);

	common::dispatcher::payload_handle payload(
		new common::dispatcher::set_actuator_state_payload(name, map.find(state)->second));
	common::dispatcher::get_dispatcher().dispatch(common::dispatcher::event_handle(
		new common::dispatcher::event(EEventType_SetActuatorState,
			static_cast<EEventScope>(area.scope), payload)));
}

} // namespace controller
#include "flow_sensor.h"

#include <csv_writer.h>

#include <config/config.h>
#include <dispatcher/event/payload/flow_sensor_state.h>
#include <dispatcher/event/payload/time_tick_payload.h>

#include <boost/bind.hpp>

namespace simulator {
namespace simulation {

flow_sensor::flow_sensor(std::string area_name_, EEventScope area_scope_,
		std::string sensor_name_, cell_handle observed_cell_) :
	logger(std::string("flow_sensor_")+sensor_name_),
	area_name(area_name_), area_scope(area_scope_),
	sensor_name(sensor_name_), observed_cell(observed_cell_),
	counter_history_limit(common::get_config().get<int>("flow_sensor_period"))
{
	time_tick_listener = common::dispatcher::get_dispatcher().register_listener(
		EEventType_TimeTick, EEventScope_General,
		boost::bind(&flow_sensor::on_time_tick, this, _1));

	common::get_csv_writer().add_key(sensor_name);
}

flow_sensor::~flow_sensor()
{
	common::dispatcher::get_dispatcher().unregister_listener(time_tick_listener);
}

const std::string& flow_sensor::get_name() const
{
	return sensor_name;
}

void flow_sensor::on_time_tick(common::dispatcher::event_handle ev)
{
	BOOST_ASSERT(EEventType_TimeTick == ev->get_type());
	BOOST_ASSERT(EEventScope_General == ev->get_scope());
	int time_tick = ev->get_payload<common::dispatcher::time_tick_payload>()->tick;

	int new_counter = observed_cell->get_vehicle_counter_value();
	counter_history.push(new_counter);
	int old_counter = counter_history.front();
	if (counter_history.size() > counter_history_limit) {
		counter_history.pop();
	}

	int passed = new_counter>=old_counter ? new_counter-old_counter : 0;
	double flow = counter_history_limit>0 ? (3600.0*passed)/counter_history_limit : 0;

	common::get_csv_writer().add_value(time_tick, sensor_name, flow);
	logger.info()() << passed << " vehicles passed in last " << counter_history_limit <<
		"s - current flow: " << flow << " vehicles per hour";

	common::dispatcher::payload_handle payload(
		new common::dispatcher::flow_sensor_state(sensor_name, flow, time_tick));
	common::dispatcher::get_dispatcher().dispatch(common::dispatcher::event_handle(
		new common::dispatcher::event(EEventType_FlowSensorState, area_scope, payload)));
}

} // namespace simulation
} // namespace simulator
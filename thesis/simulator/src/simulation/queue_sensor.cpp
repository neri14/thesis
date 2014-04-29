#include "queue_sensor.h"

#include <dispatcher/event/payload/queue_sensor_state.h>
#include <dispatcher/event/payload/time_tick_payload.h>

#include <boost/bind.hpp>

namespace simulator {
namespace simulation {

namespace constant {
const int safety_cell_count_limit(10000);
}

queue_sensor::queue_sensor(std::string area_name_, EEventScope area_scope_,
		std::string sensor_name_, cell_handle cell_from_, cell_handle cell_to_,
		int entrance_, int exit_) :
	logger(std::string("queue_sensor_")+sensor_name_),
	area_name(area_name_),
	area_scope(area_scope_),
	sensor_name(sensor_name_),
	cell_from(cell_from_),
	cell_to(cell_to_),
	entrance(entrance_),
	exit(exit_)
{
	time_tick_listener = common::dispatcher::get_dispatcher().register_listener(
		EEventType_TimeTick, EEventScope_General,
		boost::bind(&queue_sensor::on_time_tick, this, _1));
}

queue_sensor::~queue_sensor()
{
	common::dispatcher::get_dispatcher().unregister_listener(time_tick_listener);
}

const std::string& queue_sensor::get_name() const
{
	return sensor_name;
}

void queue_sensor::on_time_tick(common::dispatcher::event_handle ev)
{
	BOOST_ASSERT(EEventType_TimeTick == ev->get_type());
	BOOST_ASSERT(EEventScope_General == ev->get_scope());
	int time_tick = ev->get_payload<common::dispatcher::time_tick_payload>()->tick;

	int queue = count_occupied_cells();

	logger.info()() << "current queue " << queue;
	common::dispatcher::payload_handle payload(
		new common::dispatcher::queue_sensor_state(sensor_name, queue, time_tick));
	common::dispatcher::get_dispatcher().dispatch(common::dispatcher::event_handle(
		new common::dispatcher::event(EEventType_QueueSensorState, area_scope, payload)));
}

int queue_sensor::count_occupied_cells()
{
	int occupied = 0;
	int cnt = constant::safety_cell_count_limit;

	cell_handle c = cell_from->get_next(exit).lock();

	while (cnt && c != cell_to) {
		if (!c) {
			logger.error()() << "queue sensor not defined correctly - non existant cell";
			return 0;
		}

		if (c->is_occupied()) {
			++occupied;
		}

		c = c->get_next(0).lock();
		--cnt;
	}

	if (!cnt) {
		logger.error()() << "queue sensor not defined correctly - no path between cells";
		return 0;
	}

	if (cell_from->is_occupied()) {
		++occupied;
	}
	if (cell_to->is_occupied()) {
		++occupied;
	}

	return occupied;
}

}
}
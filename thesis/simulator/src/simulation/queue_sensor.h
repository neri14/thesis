#ifndef QUEUE_SENSOR_H
#define QUEUE_SENSOR_H

#include <boost/shared_ptr.hpp>

#include <dispatcher/event/event_types.h>
#include <dispatcher/event_dispatcher.h>

#include "cell.h"

namespace simulator {
namespace simulation {

class queue_sensor
{
public:
	queue_sensor(std::string area_name_, EEventScope area_scope_,
		std::string sensor_name_, cell_handle cell_from_, cell_handle cell_to_,
		int exit_, int entrance_);

	virtual ~queue_sensor();

	const std::string& get_name() const;

private:
	void on_time_tick(common::dispatcher::event_handle ev);

	int count_occupied_cells();
	int calculate_max_queue();

	common::my_logger logger;

	std::string area_name;
	EEventScope area_scope;
	std::string sensor_name;
	cell_handle cell_from;
	cell_handle cell_to;
	int exit_n;
	int entrance_n;

	int max_queue;

	common::dispatcher::connection_handle time_tick_listener;
};
typedef boost::shared_ptr<queue_sensor> queue_sensor_handle;

} // namespace simulation
} // namespace simulator

#endif /* QUEUE_SENSOR_H */
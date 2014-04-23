#ifndef FLOW_SENSOR_H
#define FLOW_SENSOR_H

#include <boost/shared_ptr.hpp>

#include <dispatcher/event/event_types.h>
#include <dispatcher/event_dispatcher.h>

#include "cell.h"

namespace simulator {
namespace simulation {

class flow_sensor {
public:
	flow_sensor(std::string area_name_, EEventScope area_scope_,
		std::string sensor_name_, cell_handle observed_cell_);

	virtual ~flow_sensor();

private:
	void on_time_tick(common::dispatcher::event_handle ev);

	common::my_logger logger;

	std::string area_name;
	EEventScope area_scope;
	std::string sensor_name;
	cell_handle observed_cell;

	std::queue<int> counter_history;
	int counter_history_limit;

	common::dispatcher::connection_handle time_tick_listener;
};
typedef boost::shared_ptr<flow_sensor> flow_sensor_handle;

} // namespace simulation
} // namespace simulator

#endif /* FLOW_SENSOR_H */
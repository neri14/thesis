#ifndef SIMPLE_ALGORITHM_H
#define SIMPLE_ALGORITHM_H

#include "algorithm.h"

namespace controller {

//TODO simple_algorithm
class simple_algorithm : public algorithm
{
public:
	simple_algorithm(set_actuator_cb_type cb);
	virtual ~simple_algorithm();

	virtual void on_queue_sensor_update(const std::string& name, int time_tick, int queue);
	virtual void on_flow_sensor_update(const std::string& name, int timt_tick, int flow);
	virtual void on_time_tick(int time_tick);
};

} // namespace controller

#endif /* SIMPLE_ALGORITHM_H */
#ifndef SIMPLE_ALGORITHM_H
#define SIMPLE_ALGORITHM_H

#include "algorithm.h"
#include <set>
#include <vector>

namespace controller {

struct simple_algorithm_state {
	std::set<std::string> actuators;
	int time;
};

class simple_algorithm : public algorithm
{
public:
	simple_algorithm(set_actuator_cb_type cb, controlled_area_data area_);
	virtual ~simple_algorithm();

	virtual void on_queue_sensor_update(const std::string& name, int time_tick, int queue, int max_queue);
	virtual void on_flow_sensor_update(const std::string& name, int timt_tick, int flow);
	virtual void on_time_tick(int time_tick);

private:
	void parse_algorithm_data();
	EActuatorState calculate_state(int time_tick, std::string name);
	std::string as_string(EActuatorState state);

	int cycle_time;
	std::vector<simple_algorithm_state> states;

	std::map<std::string, EActuatorState> current_state;
};

} // namespace controller

#endif /* SIMPLE_ALGORITHM_H */
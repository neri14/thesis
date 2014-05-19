#ifndef DYNAMIC_ALGORITHM_H
#define DYNAMIC_ALGORITHM_H

#include <dispatcher/event_dispatcher.h>
#include "algorithm.h"

namespace controller {

class dynamic_algorithm : public algorithm
{
public:
	dynamic_algorithm(set_actuator_cb_type cb, controlled_area_data area_);
	virtual ~dynamic_algorithm();

	virtual void on_queue_sensor_update(const std::string& name, int time_tick, int queue, int max_queue);
	virtual void on_flow_sensor_update(const std::string& name, int timt_tick, int flow);
	virtual void on_time_tick(int time_tick);

private:
	void on_expected_flow_update(common::dispatcher::event_handle ev);
	std::set<std::string> calculate_new_states();
	double calculate_weight(std::set<std::string> state_set, std::map<std::string, double> weight_map);
	std::string as_string(EActuatorState state);

	void prepare_possible_states();
	void create_possible_states(std::set<std::string> state, std::string actuator);

	void dispatch_expected_flow_events(int tick);

	int max_flow;
	int cycle_time;
	double expected_flow_factor;
	double current_flow_factor;
	double current_queue_factor;

	std::map<std::string, int> current_queue;
	std::map<std::string, int> max_queue;
	std::map<std::string, int> current_flow;
	std::map<std::string, int> expected_flow;

	std::set<std::string> penelty_weight;

	std::set<std::set<std::string> > possible_states;

	common::dispatcher::connection_handle expected_flow_listener;
};

}

#endif /* DYNAMIC_ALGORITHM_H */
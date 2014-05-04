#include "simple_algorithm.h"

#include <config/config.h>

namespace controller {

simple_algorithm::simple_algorithm(set_actuator_cb_type cb) :
	algorithm("simple_algorithm", cb),
	cycle_time(common::get_config().get<int>("cycle_time"))
{
	// TODO calculate time of every actuator in cycle - prepare fixed program
}

simple_algorithm::~simple_algorithm()
{}

void simple_algorithm::on_queue_sensor_update(const std::string& name, int time_tick, int queue, int max_queue)
{}

void simple_algorithm::on_flow_sensor_update(const std::string& name, int timt_tick, int flow)
{}

void simple_algorithm::on_time_tick(int time_tick)
{}

} // namespace controller
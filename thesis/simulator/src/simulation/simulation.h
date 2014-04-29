#ifndef SIMULATION_H
#define SIMULATION_H

#include <string>

#include <my_logger.h>

#include "../world/world_description.h"
#include "cell.h"
#include "creator.h"
#include "destroyer.h"
#include "actuator.h"
#include "flow_sensor.h"
#include "queue_sensor.h"
#include "path.h"

namespace simulator {
namespace simulation {

struct simulation_data
{
	int duration;
};

class simulation
{
public:
	simulation(const std::string& desc_filename_);
	virtual ~simulation();

	bool prepare();
	int get_duration() const;

private:
	void on_time_tick(common::dispatcher::event_handle ev);
	void on_actuator_finished(common::dispatcher::event_handle ev);
	void on_queue_sensor_state(common::dispatcher::event_handle ev);
	void on_flow_sensor_state(common::dispatcher::event_handle ev);

	void clear_pending(int time_tick, std::string identifier);
	void check_calculate_condition();

	void run_creators(int time_tick);
	void run_destroyers(int time_tick);

	bool translate_to_cell_representation(world::world_description_handle desc);

	bool translate_nodes(world::world_description_handle desc);
	bool translate_actuators(world::world_description_handle desc);
	bool translate_flow_sensors(world::world_description_handle desc);
	bool translate_queue_sensors(world::world_description_handle desc);
	bool translate_paths(world::world_description_handle desc);
	bool translate_simulation_data(world::world_description_handle desc);

	void prepare_identifiers();

	common::my_logger logger;
	std::string desc_filename;

	std::set<cell_handle> cells;
	std::map<std::string, cell_handle> cell_names;

	std::map<cell_handle, creator_handle> creators;
	std::map<cell_handle, destroyer_handle> destroyers;

	std::set<actuator_handle> actuators;
	std::set<flow_sensor_handle> flow_sensors;
	std::set<queue_sensor_handle> queue_sensors;

	std::set<path_handle> paths;
	std::map<vehicle_handle, cell_handle> vehicles;

	int simulation_duration;

	std::set<std::string> identifiers;

	int pending_time_tick;
	int last_calculated_tick;
	std::set<std::string> pending_set;
	std::set<common::dispatcher::connection_handle> listeners;

#ifdef UNIT_TEST
	friend class ut_simulation;
#endif
};

} // namespace simulation
} // namespace simulator

#endif /* SIMULATION_H */

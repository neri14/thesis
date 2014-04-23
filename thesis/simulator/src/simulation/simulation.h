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

class simulation
{
public:
	simulation(const std::string& desc_filename_);

	bool prepare();

private:
	bool translate_to_cell_representation(world::world_description_handle desc);

	bool translate_nodes(world::world_description_handle desc);
	bool translate_actuators(world::world_description_handle desc);
	bool translate_flow_sensors(world::world_description_handle desc);
	bool translate_queue_sensors(world::world_description_handle desc);
	bool translate_paths(world::world_description_handle desc);

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

#ifdef UNIT_TEST
	friend class ut_simulation;
#endif
};

} // namespace simulation
} // namespace simulator

#endif /* SIMULATION_H */

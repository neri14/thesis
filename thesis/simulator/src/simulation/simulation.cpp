#include "simulation.h"

#include "../world/world_description_parser.h"
#include "../world/world_description_validator.h"

#include <config/config.h>

namespace simulator {
namespace simulation {

simulation::simulation(const std::string& desc_filename_) :
	logger("simulation"),
	desc_filename(desc_filename_)
{}

bool simulation::prepare()
{
	logger.info()() << "Parsing world description file";
	world::world_description_parser parser(common::get_config().get<std::string>("world_description_file"));
	if (!parser.parse()) {
		return false;
	}

	logger.info()() << "Validating world description";
	world::world_description_validator validator(parser.get_world_description());
	if (!validator.validate()) {
		return false;
	}

	logger.info()() << "Translating graph representation of world description to cellular representation";
	if (!translate_to_cell_representation(parser.get_world_description())) {
		return false;
	}
	return true;
}

bool simulation::translate_to_cell_representation(world::world_description_handle desc)
{
	//TODO translating desc->nodes
	// - create cells based on nodes,exits,entrances (save priority_entrance_number)
	// - add node based cells to cell_names map
	// - add creators for nodes (connected to cells) with max_create_rate > 0
	// - add destroyers for nodes with max_destroy_rate > 0

	//TODO nodes - second pass
	// - set priority_entrance based given priority_entrance_number

	//TODO translating desc->actuators
	// - add actuators, waiting for specyfic event and controlling exit of specyfic cell

	//TODO translating desc->flow_sensors
	// - add flow sensors, checking given cell occupied flag every state(second) and counting
	//  to give flow events - for received tick event

	//TODO translating desc->queue_sensors
	// - add queue sensors, checking given set of cells for number of cars in them
	//  to give queue events - for received tick event

	//TODO translating desc->areas
	// - add areas
	// - give actuators and sensors information of their area - they need scope info for events

	//TODO translating desc->paths
	// - add paths with cells to visit queues
	// - connect paths with creators

	//TODO translating desc->simulation
	// - save settings
	// - set creators-paths rates of creation vehicles

	return true;
}

//TODO simulation::start
// - start up timer - tick event (1 second) - for event to be given (second to pass) -
//               1second timer needs to pass AND new states need to be calculated
// - creators/destroyers should work based on tick event
// - tick event should cause new state calculation

//TODO simulation::stop
// - simulation stop is based on stopping tick event generator or stops itself if duration is exceeded

} // namespace simulation
} // namespace simulator

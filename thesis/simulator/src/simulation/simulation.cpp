#include "simulation.h"

#include "../world/world_description.h"
#include "../world/world_description_parser.h"
#include "../world/world_description_validator.h"

#include <config/config.h>

#include <cmath>

#include <boost/foreach.hpp>
#include <boost/assert.hpp>

namespace simulator {
namespace simulation {

simulation::simulation(const std::string& desc_filename_) :
	logger("simulation"),
	desc_filename(desc_filename_)
{}

bool simulation::prepare()
{
	logger.info()() << "parsing world description file";
	world::world_description_parser parser(common::get_config().get<std::string>("world_description_file"));
	if (!parser.parse()) {
		return false;
	}

	logger.info()() << "validating world description";
	world::world_description_validator validator(parser.get_world_description());
	if (!validator.validate()) {
		return false;
	}

	logger.info()() << "translating graph representation of world description to cellular representation";
	if (!translate_to_cell_representation(parser.get_world_description())) {
		return false;
	}
	return true;
}

bool simulation::translate_to_cell_representation(world::world_description_handle desc)
{
	logger.info()() << "translating nodes";
	if (!translate_nodes(desc)) {
		return false;
	}

	if (!translate_actuators(desc)) {
		return false;
	}
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
	// - connect paths to creators

	//TODO translating desc->simulation
	// - save settings
	// - set creators-paths rates of creation vehicles

	return true;
}

bool simulation::translate_nodes(world::world_description_handle desc)
{
	typedef std::pair<std::string, world::world_node_handle> node_pair_t;
	//create cells based on nodes (+ add them to cell_names map)
	BOOST_FOREACH(node_pair_t node, desc->nodes) {
		cell_handle c(new cell());
		c->priority_entrance_number = node.second->priority_entrance;

		if (node.second->max_create_rate) {
			creator_handle tmp(new creator(c, node.second->max_create_rate));
			creators.insert(std::make_pair(c, tmp));
		}
		if (node.second->max_destroy_rate) {
			destroyer_handle tmp(new destroyer(c, node.second->max_destroy_rate));
			destroyers.insert(std::make_pair(c, tmp));
		}

		cells.insert(c);
		cell_names.insert(std::make_pair(node.first, c));

		logger.debug()() << "created cell for node " << node.first;
	}
	logger.debug()() << "created cells for " << cells.size() << " nodes";
	logger.debug()() << "created " << creators.size() << " creators";
	logger.debug()() << "created " << destroyers.size() << " destroyers";

	//create cells based on connection (betweent previous ones)
	BOOST_FOREACH (world::world_connection_handle conn, desc->connections) {
		world::world_node_handle node_from = conn->from.lock();
		world::world_node_handle node_to = conn->to.lock();

		cell_handle cell_from = cell_names.find(node_from->name)->second;
		cell_handle cell_to = cell_names.find(node_to->name)->second;

		int cells_count = ceil(conn->distance/desc->simulation->cell_size);

		std::pair<int, int> ends_pair = node_from->find_connection_to(*node_to);

		cell_handle last_cell = cell_from;
		for (int i=0; i<cells_count; ++i) {
			cell_handle c(new cell());
			cells.insert(c);

			int exit_num = (0 == i ? ends_pair.first : 0);
			int entr_num = 0;//(cells_count-1 == i ? ends_pair.second : 0);

			last_cell->next[exit_num] = c;
			c->prev[entr_num] = last_cell;

			last_cell = c;
		}

		last_cell->next[0] = cell_to;
		cell_to->prev[ends_pair.second] = last_cell;

		logger.debug()() << "created " << cells_count << " cells from " <<
			node_from->name << " to " << node_to->name;

		//check if cells are correct
		int counted = 0;
		last_cell = cell_from;
		for (cell_handle curr = cell_from->next[ends_pair.first].lock();
				curr && curr != cell_to; curr = curr->next[0].lock()) {
			BOOST_ASSERT(last_cell == curr->prev[0].lock());
			if (last_cell != curr->prev[0].lock()) {
				logger.error()() << "cells are not connected correctly";
				return false;
			}
			++counted;
			last_cell = curr;
		}
		if (cells_count != counted) {
			logger.error()() << "wrong number of cells created, expected: " <<
				cells_count << " there is: " << counted;
			return false;
		}
	}

	return true;
}

bool simulation::translate_actuators(world::world_description_handle desc)
{
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

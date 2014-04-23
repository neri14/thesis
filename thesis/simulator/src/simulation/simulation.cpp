#include "simulation.h"

#include "../world/world_description.h"
#include "../world/world_description_parser.h"
#include "../world/world_description_validator.h"

#include <config/config.h>

#include <cmath>

#include <boost/foreach.hpp>

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
		logger.error()() << "translating nodes failed";
		return false;
	}
	
	if (!translate_actuators(desc)) {
		logger.error()() << "translating actuators failed";
		return false;
	}

	if (!translate_flow_sensors(desc)) {
		logger.error()() << "translating flow sensors failed";
		return false;
	}

	if (!translate_queue_sensors(desc)) {
		logger.error()() << "translating queue sensors failed";
		return false;
	}

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

			last_cell->add_next(exit_num, c);
			c->add_prev(entr_num, last_cell);

			last_cell = c;
		}

		last_cell->add_next(0, cell_to);
		cell_to->add_prev(ends_pair.second, last_cell);

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
	typedef std::pair<std::string, world::world_area_handle> area_pair_t;

	BOOST_FOREACH (area_pair_t ar, desc->areas) {
		int act_count = 0;
		BOOST_FOREACH (world::world_actuator_handle act, ar.second->actuators) {
			if (cell_names.end() == cell_names.find(act->node->name)) {
				logger.error()() << "cell of given name doesn't exist (" << act->node->name << ")";
				return false;
			}

			cell_handle c = cell_names.find(act->node->name)->second;
			actuator_handle tmp(new actuator(ar.second->name,
				static_cast<EEventScope>(ar.second->scope),
				act->name, c, act->exit));
			actuators.insert(tmp);

			++act_count;
		}
		logger.debug()() << "created " << act_count << " actuators in area " << ar.first;
	}
	return true;
}

bool simulation::translate_flow_sensors(world::world_description_handle desc)
{
	typedef std::pair<std::string, world::world_area_handle> area_pair_t;

	BOOST_FOREACH (area_pair_t ar, desc->areas) {
		int sensor_count = 0;
		BOOST_FOREACH (world::world_flow_sensor_handle sens, ar.second->flow_sensors) {
			if (cell_names.end() == cell_names.find(sens->node->name)) {
				logger.error()() << "cell of given name doesn't exist (" << sens->node->name << ")";
				return false;
			}

		 	cell_handle c = cell_names.find(sens->node->name)->second;
		 	flow_sensor_handle tmp(new flow_sensor(ar.second->name,
		 		static_cast<EEventScope>(ar.second->scope), sens->name, c));
		 	flow_sensors.insert(tmp);

		 	++sensor_count;
		}
		logger.debug()() << "created " << sensor_count << " flow sensors in area " << ar.first;
	}
	return true;
}

bool simulation::translate_queue_sensors(world::world_description_handle desc)
{
	typedef std::pair<std::string, world::world_area_handle> area_pair_t;

	BOOST_FOREACH (area_pair_t ar, desc->areas) {
		int sensor_count = 0;
		BOOST_FOREACH (world::world_queue_sensor_handle sens, ar.second->queue_sensors) {
			if (cell_names.end() == cell_names.find(sens->node_from->name)) {
				logger.error()() << "cell of given name doesn't exist (" << sens->node_from->name << ")";
				return false;
			}
			if (cell_names.end() == cell_names.find(sens->node_to->name)) {
				logger.error()() << "cell of given name doesn't exist (" << sens->node_to->name << ")";
				return false;
			}

		 	cell_handle c_from = cell_names.find(sens->node_from->name)->second;
		 	cell_handle c_to = cell_names.find(sens->node_to->name)->second;

		 	int c_from_exit = sens->node_from_exit;
		 	int c_to_entrance = sens->node_to_entrance;

		 	queue_sensor_handle tmp(new queue_sensor(ar.second->name,
		 		static_cast<EEventScope>(ar.second->scope), sens->name, c_from, c_to,
		 		c_from_exit, c_to_entrance));
		 	queue_sensors.insert(tmp);

		 	++sensor_count;
		}
		logger.debug()() << "created " << sensor_count << " queue sensors in area " << ar.first;
	}
	return true;
}

//TODO simulation::start
// - start up timer - tick event (1 second) - for event to be given (second to pass) -
//               1second timer needs to pass AND new states need to be calculated
// - creators/destroyers should work based on tick event
// - tick event should cause new state calculation - after actuators are finished and sensors sent their events

//TODO simulation::stop
// - simulation stop is based on stopping tick event generator or stops itself if duration is exceeded

} // namespace simulation
} // namespace simulator

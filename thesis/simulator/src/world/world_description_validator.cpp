#include "world_description_validator.h"
#include <boost/foreach.hpp>

namespace simulator {
namespace world {

world_description_validator::world_description_validator(world_description_handle desc_) :
	logger("world_description_validator"),
	desc(desc_)
{}

bool world_description_validator::validate()
{
	bool result = true;
	result = check_loose_ends() && result;
	result = check_paths_integrity() && result;
	result = check_actuators_area_membership() && result;
	result = check_flow_sensors_area_membership() && result;
	result = check_queue_sensors_area_membership() && result;

	if (result) {
		logger.info()() << "world description is correct";
	}
	return result;
}

bool world_description_validator::check_loose_ends()
{
	typedef std::pair<std::string, world_node_handle> node_pair_t;
	typedef std::pair<int, world_connection_handle> conn_pair_t;

	bool result = true;
	BOOST_FOREACH(node_pair_t pair, desc->nodes) {
		BOOST_FOREACH(conn_pair_t end, pair.second->entrances) {
			if (!end.second) {
				logger.error()() << "loose end in node: " << pair.second->name;
				result = false;
			}
		}
		BOOST_FOREACH(conn_pair_t end, pair.second->exits) {
			if (!end.second) {
				logger.error()() << "loose end in node: " << pair.second->name;
				result = false;
			}
		}
	}
	BOOST_FOREACH(world_connection_handle conn, desc->connections) {
		if (!conn->from.lock() || !conn->to.lock()) {
			logger.error()() << "loose end in connection";
			result = false;
		}
	}

	if (result) {
		logger.debug()() << "no loose ends";
	}
	return result;
}

bool world_description_validator::check_paths_integrity()
{
	typedef std::pair<std::string, world_path_handle> path_pair_t;

	bool result = true;
	BOOST_FOREACH(path_pair_t path, desc->paths) {
		if (path.second->get_length() < 0) {
			logger.error()() << "can't determine path length - path not ok: " << path.second->name;
			result = false;
		}
	}

	if (result) {
		logger.debug()() << "all paths are ok";
	}
	return result;
}

bool world_description_validator::check_actuators_area_membership()
{
	typedef std::pair<std::string, world_actuator_handle> act_pair_t;
	typedef std::pair<std::string, world_area_handle> area_pair_t;

	bool result = true;
	BOOST_FOREACH(act_pair_t act, desc->actuators) {
		bool member = false;
		BOOST_FOREACH(area_pair_t area, desc->areas) {
			if (area.second->actuators.find(act.second) != area.second->actuators.end()) {
				member = true;
			}
		}

		if (!member) {
			logger.error()() << "actuator is not a member of any area: " << act.second->name;
			result = false;
		}
	}
	if (result) {
		logger.debug()() << "all actuators are members of areas";
	}
	return result;
}

bool world_description_validator::check_flow_sensors_area_membership()
{
	typedef std::pair<std::string, world_flow_sensor_handle> flow_pair_t;
	typedef std::pair<std::string, world_area_handle> area_pair_t;

	bool result = true;
	BOOST_FOREACH(flow_pair_t flow, desc->flow_sensors) {
		bool member = false;
		BOOST_FOREACH(area_pair_t area, desc->areas) {
			if (area.second->flow_sensors.find(flow.second) != area.second->flow_sensors.end()) {
				member = true;
			}
		}

		if (!member) {
			logger.error()() << "flow sensor is not a member of any area: " << flow.second->name;
			result = false;
		}
	}
	if (result) {
		logger.debug()() << "all flow sensors are members of areas";
	}
	return result;
}

bool world_description_validator::check_queue_sensors_area_membership()
{
	typedef std::pair<std::string, world_queue_sensor_handle> que_pair_t;
	typedef std::pair<std::string, world_area_handle> area_pair_t;

	bool result = true;
	BOOST_FOREACH(que_pair_t que, desc->queue_sensors) {
		bool member = false;
		BOOST_FOREACH(area_pair_t area, desc->areas) {
			if (area.second->queue_sensors.find(que.second) != area.second->queue_sensors.end()) {
				member = true;
			}
		}

		if (!member) {
			logger.error()() << "queue sensor is not a member of any area: " << que.second->name;
			result = false;
		}
	}
	if (result) {
		logger.debug()() << "all queue sensors are members of areas";
	}
	return result;
}

} // namespace world
} // namespace simulator

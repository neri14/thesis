#ifndef WORLD_DESCRIPTION_PARSER_H
#define WORLD_DESCRIPTION_PARSER_H

#include "world_description.h"

#include <my_logger.h>

#include <string>
#include <queue>
#include <boost/property_tree/ptree.hpp>

namespace simulator {
namespace world {

class world_description_parser
{
public:
	world_description_parser(const std::string& filename);
	virtual ~world_description_parser();

	bool parse();
	world_description_handle get_world_description();

private:
	bool parse_file(const std::string& file);

	bool parse_world(const std::string& key, const boost::property_tree::ptree& pt);

	bool parse_files(const boost::property_tree::ptree& pt);
	bool parse_nodes(const boost::property_tree::ptree& pt);
	bool parse_connections(const boost::property_tree::ptree& pt);
	bool parse_creators(const boost::property_tree::ptree& pt);
	bool parse_destroyers(const boost::property_tree::ptree& pt);
	bool parse_actuators(const boost::property_tree::ptree& pt);
	bool parse_flow_sensors(const boost::property_tree::ptree& pt);
	bool parse_queue_sensors(const boost::property_tree::ptree& pt);
	bool parse_areas(const boost::property_tree::ptree& pt);
	bool parse_paths(const boost::property_tree::ptree& pt);
	bool parse_priorities(const boost::property_tree::ptree& pt);
	bool parse_simulation(const boost::property_tree::ptree& pt);

	std::pair<int,int> find_connection(world_node_handle node_from, world_node_handle node_to);

	common::my_logger logger;
	std::queue<std::string> files;

	world_description_handle desc;
};

} // namespace world
} // namespace simulator

#endif /* WORLD_DESCRIPTION_PARSER_H */

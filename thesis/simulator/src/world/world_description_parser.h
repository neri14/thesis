#ifndef WORLD_DESCRIPTION_PARSER_H
#define WORLD_DESCRIPTION_PARSER_H

#include "world_description.h"

#include <my_logger.h>

#include <string>
#include <queue>
#include <boost/property_tree/ptree.hpp>

namespace simulator {
namespace world {

struct wd_direction
{
	std::string entrance_name;

	std::map <std::string, int> connection;
	std::map <int, int> flows; //time, outflow
};

class world_description_parser
{
public:
	world_description_parser(const std::string& filename);
	virtual ~world_description_parser();

	bool parse();
	world_description_handle get_world_description() const;

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
	bool parse_directions_connections(const boost::property_tree::ptree& pt);
	bool parse_priorities(const boost::property_tree::ptree& pt);
	bool parse_simulation(const boost::property_tree::ptree& pt);

	bool create_paths();
	std::vector<std::string> find_path(const std::string& from, const std::string& to,
		std::vector<std::string> visited = std::vector<std::string>());
	std::string add_path(
		const std::string& from, const std::string& to, const std::vector<std::string>& nodes);
	void add_path_flow(const std::string& path, int time, int flow);

	common::my_logger logger;
	std::queue<std::string> files;

	world_description_handle desc;
	std::map<std::string, wd_direction> directions;
};

} // namespace world
} // namespace simulator

#endif /* WORLD_DESCRIPTION_PARSER_H */

#include "world_description_parser.h"

#include <boost/assign.hpp>
#include <boost/foreach.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/filesystem.hpp>

namespace simulator {
namespace world {

world_description_parser::world_description_parser(const std::string& filename) :
	logger("world_description_parser"),
	desc(new world_description())
{
	files.push(filename);
}

world_description_parser::~world_description_parser()
{}

bool world_description_parser::parse()
{
	bool status = true;
	while (status && !files.empty()) {
		std::string file = files.front();
		status = parse_file(file);
		files.pop();
	}
	return status;
}

world_description_handle world_description_parser::get_world_description()
{
	return desc;
}

bool world_description_parser::parse_file(const std::string& file)
{
	logger.info()() << "parsing file: " << file;
	try {
		using boost::property_tree::ptree;
		ptree pt;

		read_xml(file, pt);

		BOOST_FOREACH(const ptree::value_type& v, pt.get_child("world")) {
			if (!parse_world(v.first, v.second)) {
				return false;
			}
		}
	} catch (std::exception& e) {
		logger.error()() << "parsing error: " << e.what();
		return false;
	}
	logger.debug()() << "parsed file: " << file;
	return true;
}

bool world_description_parser::parse_world(const std::string& key, const boost::property_tree::ptree& pt)
{
	bool res = true;
	if ("files" == key) {
		res = parse_files(pt);
	} else if ("nodes" == key) {
		res = parse_nodes(pt);
	} else if ("connections" == key) {
		res = parse_connections(pt);
	} else if ("creators" == key) {
		res = parse_creators(pt);
	} else if ("destroyers" == key) {
		res = parse_destroyers(pt);
	} else if ("actuators" == key) {
		res = parse_actuators(pt);
	} else if ("flow_sensors" == key) {
		res = parse_flow_sensors(pt);
	} else if ("queue_sensors" == key) {
		res = parse_queue_sensors(pt);
	} else if ("areas" == key) {
		res = parse_areas(pt);
	} else if ("paths" == key) {
		res = parse_paths(pt);
	} else if ("priorities" == key) {
		res = parse_priorities(pt);
	} else if ("simulation" == key) {
		res = parse_simulation(pt);
	}
	return res;
}

bool world_description_parser::parse_files(const boost::property_tree::ptree& pt)
{
	using boost::property_tree::ptree;
	BOOST_FOREACH(const ptree::value_type& v, pt) {
		if ("file" == v.first) {
			boost::filesystem::path current_file(files.front());
			boost::filesystem::path current_dir = current_file.parent_path();
			files.push(current_dir.string() + "/"+ v.second.data());
			logger.debug()() << "added file to parse: " << v.second.data();
		} else {
			logger.warning()() << "unexpected xml tag: " << v.first;
		}
	}
	return true;
}

bool world_description_parser::parse_nodes(const boost::property_tree::ptree& pt)
{
	typedef std::map<std::string, ENodeType> mapping_t;
	static mapping_t mapping = boost::assign::map_list_of
		("ENTRANCE",    ENodeType_Entrance)
		("EXIT",        ENodeType_Exit)
		("SPLIT",       ENodeType_Split)
		("JOIN",        ENodeType_Join)
		("INTERSECT",   ENodeType_Intersect)
		("SIMPLE",      ENodeType_Simple);

	using boost::property_tree::ptree;
	BOOST_FOREACH(const ptree::value_type& v, pt) {
		if ("node" == v.first) {
			std::string type = v.second.get<std::string>("type");
			mapping_t::const_iterator it = mapping.find(type);

			if (it != mapping.end()) {
				world_node_handle n(new world_node(v.second.get<std::string>("name"), it->second));

				if (desc->nodes.insert(std::make_pair(n->name, n)).second) {
					logger.debug()() << "added node (" << n->name << "," << type << ")";
				} else {
					logger.error()() << "duplicated node name: " << n->name;
					return false;
				}
			} else {
				logger.error()() << "wrong node type: " << type;
				return false;
			}
		} else {
			logger.warning()() << "unexpected xml tag: " << v.first;
		}
	}
	return true;
}

bool world_description_parser::parse_connections(const boost::property_tree::ptree& pt)
{
	using boost::property_tree::ptree;
	BOOST_FOREACH(const ptree::value_type& v, pt) {
		if ("connection" == v.first) {
			std::string from_name = v.second.get<std::string>("node_from.name");
			int from_exit = v.second.get<int>("node_from.exit");
			std::string to_name = v.second.get<std::string>("node_to.name");
			int to_entrance = v.second.get<int>("node_to.entrance");
			int distance = v.second.get<int>("distance");

			world_node_handle node_from;
			if (desc->nodes.find(from_name) != desc->nodes.end()) {
				node_from = desc->nodes.find(from_name)->second;
			} else {
				logger.error()() << "wrong node name: " << from_name;
				return false;
			}

			world_node_handle node_to;
			if (desc->nodes.find(to_name) != desc->nodes.end()) {
				node_to = desc->nodes.find(to_name)->second;
			} else {
				logger.error()() << "wrong node name: " << to_name;
				return false;
			}

			if (!node_from->exits.count(from_exit)) {
				logger.error()() << "wrong exit for given node: (" << from_name << "," << from_exit << ")";
				return false;
			}
			if (!node_to->entrances.count(to_entrance)) {
				logger.error()() << "wrong entrance for given node: (" << to_name << "," << to_entrance << ")";
				return false;
			}

			world_connection_handle conn(new world_connection(distance));

			if (!node_from->exits[from_exit]) {
				node_from->exits[from_exit] = conn;
			} else {
				logger.error()() << "multiple use of exit: (" << from_name << "," << from_exit << ")";
				return false;
			}
			if (!node_to->entrances[to_entrance]) {
				node_to->entrances[to_entrance] = conn;
			} else {
				logger.error()() << "multiple use of entrance: (" << to_name << "," << to_entrance << ")";
				return false;
			}

			logger.debug()() << "added connection (" << from_name << "," << from_exit << ") -> (" <<
				to_name << "," << to_entrance << ")";
		} else {
			logger.warning()() << "unexpected xml tag: " << v.first;
		}
	}
	return true;
}

bool world_description_parser::parse_creators(const boost::property_tree::ptree& pt)
{
	using boost::property_tree::ptree;
	BOOST_FOREACH(const ptree::value_type& v, pt) {
		if ("creator" == v.first) {
			std::string node = v.second.get<std::string>("node_name");
			int rate = v.second.get<int>("create_rate");
			if (rate < 0) {
				rate = 0;
			}

			if (desc->nodes.find(node) != desc->nodes.end()) {
				desc->nodes.find(node)->second->max_create_rate = rate;
				logger.debug()() << "set node " << node << " max create rate to " << rate;
			} else {
				logger.error()() << "wrong node name: " << node;
				return false;
			}
		} else {
			logger.warning()() << "unexpected xml tag: " << v.first;
		}
	}
	return true;
}

bool world_description_parser::parse_destroyers(const boost::property_tree::ptree& pt)
{
	using boost::property_tree::ptree;
	BOOST_FOREACH(const ptree::value_type& v, pt) {
		if ("destroyer" == v.first) {
			std::string node = v.second.get<std::string>("node_name");
			int rate = v.second.get<int>("destroy_rate");
			if (rate < 0) {
				rate = 0;
			}

			if (desc->nodes.find(node) != desc->nodes.end()) {
				desc->nodes.find(node)->second->max_destroy_rate = rate;
				logger.debug()() << "set node " << node << " max destroy rate to " << rate;
			} else {
				logger.error()() << "wrong node name: " << node;
				return false;
			}
		} else {
			logger.warning()() << "unexpected xml tag: " << v.first;
		}
	}
	return true;
}

bool world_description_parser::parse_actuators(const boost::property_tree::ptree& pt) {return true;}
//TODO parse_actuators
bool world_description_parser::parse_flow_sensors(const boost::property_tree::ptree& pt) {return true;}
//TODO parse_flow_sensors
bool world_description_parser::parse_queue_sensors(const boost::property_tree::ptree& pt) {return true;}
//TODO parse_queue_sensors
bool world_description_parser::parse_areas(const boost::property_tree::ptree& pt) {return true;}
//TODO parse_areas
bool world_description_parser::parse_paths(const boost::property_tree::ptree& pt) {return true;}
//TODO parse_paths
bool world_description_parser::parse_priorities(const boost::property_tree::ptree& pt) {return true;}
//TODO parse_priorities
bool world_description_parser::parse_simulation(const boost::property_tree::ptree& pt) {return true;}
//TODO parse_simulation

} // namespace world
} // namespace simulator

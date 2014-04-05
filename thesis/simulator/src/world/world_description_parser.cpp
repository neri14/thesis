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

			if (it == mapping.end()) {
				logger.error()() << "wrong node type: " << type;
				return false;
			}

			world_node_handle n(new world_node(v.second.get<std::string>("name"), it->second));

			if (!desc->nodes.insert(std::make_pair(n->name, n)).second) {
				logger.error()() << "duplicated node name: " << n->name;
				return false;
			}

			logger.debug()() << "added node (" << n->name << "," << type << ")";
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
			world_node_handle node_to;

			if (desc->nodes.find(from_name) == desc->nodes.end()) {
				logger.error()() << "wrong node name: " << from_name;
				return false;
			}

			if (desc->nodes.find(to_name) == desc->nodes.end()) {
				logger.error()() << "wrong node name: " << to_name;
				return false;
			}

			node_from = desc->nodes.find(from_name)->second;
			node_to = desc->nodes.find(to_name)->second;

			if (!node_from->exits.count(from_exit)) {
				logger.error()() << "wrong exit for given node: (" << from_name << "," << from_exit << ")";
				return false;
			}

			if (!node_to->entrances.count(to_entrance)) {
				logger.error()() << "wrong entrance for given node: (" << to_name << "," << to_entrance << ")";
				return false;
			}

			world_connection_handle conn(new world_connection(distance));

			if (node_from->exits[from_exit]) {
				logger.error()() << "multiple use of exit: (" << from_name << "," << from_exit << ")";
				return false;
			}

			if (node_to->entrances[to_entrance]) {
				logger.error()() << "multiple use of entrance: (" << to_name << "," << to_entrance << ")";
				return false;
			}

			node_from->exits[from_exit] = conn;
			node_to->entrances[to_entrance] = conn;
			desc->connections.insert(conn);
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

			if (desc->nodes.find(node) == desc->nodes.end()) {
				logger.error()() << "wrong node name: " << node;
				return false;
			}

			desc->nodes.find(node)->second->max_create_rate = rate;
			logger.debug()() << "set node " << node << " max create rate to " << rate;
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

			if (desc->nodes.find(node) == desc->nodes.end()) {
				logger.error()() << "wrong node name: " << node;
				return false;
			}

			desc->nodes.find(node)->second->max_destroy_rate = rate;
			logger.debug()() << "set node " << node << " max destroy rate to " << rate;
		} else {
			logger.warning()() << "unexpected xml tag: " << v.first;
		}
	}
	return true;
}

bool world_description_parser::parse_actuators(const boost::property_tree::ptree& pt)
{
	using boost::property_tree::ptree;
	BOOST_FOREACH(const ptree::value_type& v, pt) {
		if ("actuator" == v.first) {
			std::string name = v.second.get<std::string>("name");
			std::string node = v.second.get<std::string>("node_name");
			int exit = v.second.get<int>("exit_number");

			if (desc->nodes.find(node) == desc->nodes.end()) {
				logger.error()() << "wrong node name: " << node;
				return false;
			}

			world_node_handle n = desc->nodes.find(node)->second;

			if (n->exits.find(exit) == n->exits.end()) {
				logger.error()() << "wrong node exit: (" << node << "," << exit << ")";
				return false;
			}

			if (desc->actuators.find(name) != desc->actuators.end()) {
				logger.error()() << "duplicated actuator name: " << name;
				return false;
			}

			desc->actuators.insert(std::make_pair(name,
				world_actuator_handle(new world_actuator(name, n, exit))));
			logger.debug()() << "added actuator: (" << name << ") -> (" << node << "," << exit << ")";
		} else {
			logger.warning()() << "unexpected xml tag: " << v.first;
		}
	}
	return true;
}

bool world_description_parser::parse_flow_sensors(const boost::property_tree::ptree& pt)
{
	using boost::property_tree::ptree;
	BOOST_FOREACH(const ptree::value_type& v, pt) {
		if ("flow_sensor" == v.first) {
			std::string name = v.second.get<std::string>("name");
			std::string node = v.second.get<std::string>("node_name");
			int exit = v.second.get<int>("exit_number");

			if (desc->nodes.find(node) == desc->nodes.end()) {
				logger.error()() << "wrong node name: " << node;
				return false;
			}

			world_node_handle n = desc->nodes.find(node)->second;

			if (n->exits.find(exit) == n->exits.end()) {
				logger.error()() << "wrong node exit: (" << node << "," << exit << ")";
				return false;
			}

			if (desc->flow_sensors.find(name) != desc->flow_sensors.end() ||
					desc->queue_sensors.find(name) != desc->queue_sensors.end()) {
				logger.error()() << "duplicated sensor name: " << name;
				return false;
			}

			desc->flow_sensors.insert(std::make_pair(name,
				world_flow_sensor_handle(new world_flow_sensor(name, n, exit))));
			logger.debug()() << "added flow sensor: (" << node << "," << exit << ") -> (" << name << ")";
		} else {
			logger.warning()() << "unexpected xml tag: " << v.first;
		}
	}
	return true;
}

bool world_description_parser::parse_queue_sensors(const boost::property_tree::ptree& pt)
{
	using boost::property_tree::ptree;
	BOOST_FOREACH(const ptree::value_type& v, pt) {
		if ("queue_sensor" == v.first) {
			std::string name = v.second.get<std::string>("name");
			std::string from = v.second.get<std::string>("node_from");
			std::string to = v.second.get<std::string>("node_to");

			if (desc->nodes.find(from) == desc->nodes.end()) {
				logger.error()() << "wrong node name: " << from;
				return false;
			}
			if (desc->nodes.find(to) == desc->nodes.end()) {
				logger.error()() << "wrong node name: " << to;
				return false;
			}

			world_node_handle node_from = desc->nodes.find(from)->second;
			world_node_handle node_to = desc->nodes.find(to)->second;

			std::pair<int,int> tmp_pair = node_from->find_connection_to(*node_to);
			int exit = tmp_pair.first;
			int entrance = tmp_pair.second;

			if (exit == -1 || entrance == -1) {
				logger.error()() << "could not find connection between nodes: " << from << " -> " << to;
				return false;
			}

			if (desc->flow_sensors.find(name) != desc->flow_sensors.end() ||
					desc->queue_sensors.find(name) != desc->queue_sensors.end()) {
				logger.error()() << "duplicated sensor name: " << name;
				return false;
			}

			desc->queue_sensors.insert(std::make_pair(name, world_queue_sensor_handle(
				new world_queue_sensor(name, node_from, node_to, exit, entrance))));

			logger.debug()() << "added queue sensor: (" << from << ") - (" << to << ") -> (" << name << ")";
		} else {
			logger.warning()() << "unexpected xml tag: " << v.first;
		}
	}
	return true;
}

bool world_description_parser::parse_areas(const boost::property_tree::ptree& pt)
{
	using boost::property_tree::ptree;
	BOOST_FOREACH(const ptree::value_type& v, pt) {
		if ("area" == v.first) {
			std::string area_name = v.second.get<std::string>("name");
			int scope = v.second.get<int>("scope");

			if (desc->areas.find(area_name) != desc->areas.end()) {
				logger.error()() << "duplicated area name: " << area_name;
				return false;
			}
			world_area_handle area(new world_area(area_name, scope));

			BOOST_FOREACH(const ptree::value_type& tree, v.second.get_child("actuators")) {
				if ("name" == tree.first) {
					if (desc->actuators.end() == desc->actuators.find(tree.second.data())) {
						logger.error()() << "wrong actuator name: " << tree.second.data();
						return false;
					}
					area->actuators.insert(desc->actuators.find(tree.second.data())->second);
				} else {
					logger.warning()() << "unexpected xml tag: " << tree.first;
				}
			}
			BOOST_FOREACH(const ptree::value_type& tree, v.second.get_child("flow_sensors")) {
				if ("name" == tree.first) {
					if (desc->flow_sensors.end() == desc->flow_sensors.find(tree.second.data())) {
						logger.error()() << "wrong flow sensor name: " << tree.second.data();
						return false;
					}
					area->flow_sensors.insert(desc->flow_sensors.find(tree.second.data())->second);
				} else {
					logger.warning()() << "unexpected xml tag: " << tree.first;
				}
			}
			BOOST_FOREACH(const ptree::value_type& tree, v.second.get_child("queue_sensors")) {
				if ("name" == tree.first) {
					if (desc->queue_sensors.end() == desc->queue_sensors.find(tree.second.data())) {
						logger.error()() << "wrong queue sensor name: " << tree.second.data();
						return false;
					}
					area->queue_sensors.insert(desc->queue_sensors.find(tree.second.data())->second);
				} else {
					logger.warning()() << "unexpected xml tag: " << tree.first;
				}
			}

			desc->areas.insert(std::make_pair(area_name, area));
			logger.debug()() << "added area: " << area_name << ", scope: " << scope <<
				" (" << area->actuators.size() << " actuators, " << area->flow_sensors.size() <<
				" flow sensors, " << area->queue_sensors.size() << " queue sensors)";
		} else {
			logger.warning()() << "unexpected xml tag: " << v.first;
		}
	}
	return true;
}

bool world_description_parser::parse_paths(const boost::property_tree::ptree& pt)
{
	using boost::property_tree::ptree;
	BOOST_FOREACH(const ptree::value_type& v, pt) {
		if ("path" == v.first) {
			std::string path_name = v.second.get<std::string>("name");

			if (desc->paths.find(path_name) != desc->paths.end()) {
				logger.error()() << "duplicated path name: " << path_name;
				return false;
			}
			world_path_handle path(new world_path(path_name));

			BOOST_FOREACH(const ptree::value_type& tree, v.second.get_child("nodes")) {
				if ("node" == tree.first) {
					if (desc->nodes.end() == desc->nodes.find(tree.second.data())) {
						logger.error()() << "wrong node name: " << tree.second.data();
						return false;
					}
					path->nodes.push_back(desc->nodes.find(tree.second.data())->second);
				} else {
					logger.warning()() << "unexpected xml tag: " << tree.first;
				}
			}

			desc->paths.insert(std::make_pair(path_name, path));
			logger.debug()() << "added path: " << path_name << ", nodes: " << path->nodes.size() <<
				" length: " << path->get_length();
		} else {
			logger.warning()() << "unexpected xml tag: " << v.first;
		}
	}
	return true;
}

bool world_description_parser::parse_priorities(const boost::property_tree::ptree& pt)
{
	using boost::property_tree::ptree;
	BOOST_FOREACH(const ptree::value_type& v, pt) {
		if ("priority" == v.first) {
			std::string node_name = v.second.get<std::string>("node_name");
			int entrance = v.second.get<int>("entrance");

			if (desc->nodes.find(node_name) == desc->nodes.end()) {
				logger.error()() << "wrong node name: " << node_name;
				return false;
			}

			world_node_handle node = desc->nodes.find(node_name)->second;

			if (entrance < 0 || node->entrances.size() <= entrance) {
				logger.error()() << "wrong entrance number: " << entrance;
				return false;
			}

			node->priority_entrance = entrance;
			logger.debug()() << "node " << node_name << " priority entrance set to " << entrance;
		} else {
			logger.warning()() << "unexpected xml tag: " << v.first;
		}
	}
	return true;
}

bool world_description_parser::parse_simulation(const boost::property_tree::ptree& pt)
{
	using boost::property_tree::ptree;
	desc->simulation.reset(new world_simulation());

	BOOST_FOREACH(const ptree::value_type& v, pt) {
		if ("duration" == v.first) {
			desc->simulation->duration = v.second.get_value<int>();
		} else if ("cell_size" == v.first) {
			desc->simulation->cell_size = v.second.get_value<double>();
		} else if ("max_speed" == v.first) {
			desc->simulation->max_speed = v.second.get_value<int>();
		} else if ("path_flows" == v.first) {
			BOOST_FOREACH(const ptree::value_type& tree, v.second) {
				if ("path_flow" == tree.first) {
					std::string path_name = tree.second.get<std::string>("path_name");
					if (desc->paths.end() == desc->paths.find(path_name)) {
						logger.error()() << "wrong path name: " << tree.second.data();
						return false;
					}

					world_path_handle path = desc->paths.find(path_name)->second;
					int start_time = tree.second.get<int>("start_time");
					int flow = tree.second.get<int>("flow");

					desc->simulation->path_flows.insert(
						std::make_pair(start_time, std::make_pair(flow, path)));
					logger.debug()() << "added path flow, start_time: " << start_time <<
						", flow: " << flow << ", path: " << path_name;
				} else {
					logger.warning()() << "unexpected xml tag: " << tree.first;
				}
			}
		} else {
			logger.warning()() << "unexpected xml tag: " << v.first;
		}
	}

	logger.debug()() << "simulation duration: " << desc->simulation->duration <<
		"s, path flows number: " << desc->simulation->path_flows.size();
	return true;
}

} // namespace world
} // namespace simulator

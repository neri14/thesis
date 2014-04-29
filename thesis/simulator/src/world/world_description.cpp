#include "world_description.h"

#include <boost/foreach.hpp>

namespace simulator {
namespace world {

world_connection::world_connection(int distance_) :
	distance(distance_)
{}

world_node::world_node(const std::string& name_, ENodeType type_) :
	type(type_),
	name(name_),
	max_create_rate(0),
	max_destroy_rate(0),
	priority_entrance(0)
{
	int in_cnt = 0;
	int out_cnt = 0;

	switch (type) {
		case ENodeType_Entrance :
			in_cnt = 0;
			out_cnt = 1;
			break;
		case ENodeType_Exit :
			in_cnt = 1;
			out_cnt = 0;
			break;
		case ENodeType_Split :
			in_cnt = 1;
			out_cnt = 2;
			break;
		case ENodeType_Join :
			in_cnt = 2;
			out_cnt = 1;
			break;
		case ENodeType_Intersect :
			in_cnt = 2;
			out_cnt = 2;
			break;
		case ENodeType_Simple :
			in_cnt = 1;
			out_cnt = 1;
			break;
	}

	for (int i=0; i<in_cnt; ++i) {
		entrances.insert(std::make_pair(i, world_connection_handle()));
	}
	for (int i=0; i<out_cnt; ++i) {
		exits.insert(std::make_pair(i, world_connection_handle()));
	}
}

std::pair<int,int> world_node::find_connection_to(const world_node& node_to) const
{
	int exit = -1;
	int entrance = -1;

	typedef std::pair<int, world_connection_handle> conn_pair_type;
	BOOST_FOREACH (conn_pair_type conn_from, exits) {
		BOOST_FOREACH (conn_pair_type conn_to, node_to.entrances) {
			if (conn_from.second == conn_to.second) {
				exit = conn_from.first;
				entrance = conn_to.first;
			}
		}
	}
	
	return std::make_pair(exit, entrance);
}

world_actuator::world_actuator(const std::string& name_, world_node_handle node_, int exit_) :
	name(name_), node(node_), exit(exit_)
{}

world_flow_sensor::world_flow_sensor(const std::string& name_, world_node_handle node_) :
	name(name_), node(node_)
{}

world_queue_sensor::world_queue_sensor(const std::string& name_, world_node_handle node_from_,
		world_node_handle node_to_, int node_from_exit_, int node_to_entrance_) :
	name(name_), node_from(node_from_), node_to(node_to_),
	node_from_exit(node_from_exit_), node_to_entrance(node_to_entrance_)
{}

world_area::world_area(const std::string& name_, int scope_) :
	name(name_), scope(scope_)
{}

world_path::world_path(const std::string& name_) :
	name(name_)
{}

int world_path::get_length() const
{
	int sum = 0;
	try {
		world_node_handle prev;
		BOOST_FOREACH(world_node_handle node, nodes) {
			if (prev) {
				sum += node->entrances.find(prev->find_connection_to(*node).second)->second->distance;
			}
			prev = node;
		}
	} catch (std::exception&) {
		return -1;
	}
	return sum;
}

world_simulation::world_simulation() :
	duration(0), cell_size(0), max_speed(0)
{}

}// namespace simulator
}// namespace world

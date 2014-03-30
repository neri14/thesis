#include "world_description.h"

namespace simulator {
namespace world {

world_connection::world_connection(int distance_) :
	distance(distance_)
{}

world_node::world_node(const std::string& name_, ENodeType type_) :
	type(type_),
	name(name_),
	max_create_rate(0),
	max_destroy_rate(0)
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

world_actuator::world_actuator(const std::string& name_, world_node_handle node_, int exit_) :
	name(name_), node(node_), exit(exit_)
{}

world_flow_sensor::world_flow_sensor(const std::string& name_, world_node_handle node_, int exit_) :
	name(name_), node(node_), exit(exit_)
{}

world_queue_sensor::world_queue_sensor(const std::string& name_, world_node_handle node_from_,
		world_node_handle node_to_, int node_from_exit_, int node_to_entrance_) :
	name(name_), node_from(node_from_), node_to(node_to_),
	node_from_exit(node_from_exit_), node_to_entrance(node_to_entrance_)
{}

}// namespace simulator
}// namespace world

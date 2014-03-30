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

}// namespace simulator
}// namespace world

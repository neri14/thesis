#ifndef WORLD_DESCRIPTION_H
#define WORLD_DESCRIPTION_H

#include <map>
#include <set>
#include <string>
#include <boost/shared_ptr.hpp>

namespace simulator {
namespace world {

enum ENodeType
{
	ENodeType_Entrance,
	ENodeType_Exit,
	ENodeType_Split,
	ENodeType_Join,
	ENodeType_Intersect,
	ENodeType_Simple
};

struct world_connection
{
	world_connection(int distance_);

	int distance;
};
typedef boost::shared_ptr<world_connection> world_connection_handle;

struct world_node
{
	world_node(const std::string& name_, ENodeType type_);
	std::string name;
	ENodeType type;
	int max_create_rate;
	int max_destroy_rate;

	std::map<int, world_connection_handle> entrances;
	std::map<int, world_connection_handle> exits;
};
typedef boost::shared_ptr<world_node> world_node_handle;

struct world_actuator
{
	world_actuator(const std::string& name_, world_node_handle node_, int exit_);

	std::string name;
	world_node_handle node;
	int exit;
};
typedef boost::shared_ptr<world_actuator> world_actuator_handle;

struct world_flow_sensor
{
	world_flow_sensor(const std::string& name_, world_node_handle node_, int exit_);

	std::string name;
	world_node_handle node;
	int exit;
};
typedef boost::shared_ptr<world_flow_sensor> world_flow_sensor_handle;

struct world_queue_sensor
{
	world_queue_sensor(const std::string& name_, world_node_handle node_from_, world_node_handle node_to_,
		int node_from_exit_, int node_to_entrance_);

	std::string name;
	world_node_handle node_from;
	world_node_handle node_to;
	int node_from_exit;
	int node_to_entrance;
};
typedef boost::shared_ptr<world_queue_sensor> world_queue_sensor_handle;

struct world_description
{
	std::map<std::string, world_node_handle> nodes;
	std::set<world_connection_handle> connections;
	std::map<std::string, world_actuator_handle> actuators;
	std::map<std::string, world_flow_sensor_handle> flow_sensors;
	std::map<std::string, world_queue_sensor_handle> queue_sensors;
};
typedef boost::shared_ptr<world_description> world_description_handle;

} // namespace world
} // namespace simulator

#endif /* WORLD_DESCRIPTION_H */

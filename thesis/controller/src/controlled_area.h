#ifndef CONTROLLED_AREA_H
#define CONTROLLED_AREA_H

#include <my_logger.h>

#include <map>
#include <set>
#include <vector>
#include <boost/optional.hpp>
#include <boost/property_tree/ptree.hpp>

namespace controller {

struct detailed_data {
	boost::optional<std::string> flow_sensor;
	boost::optional<std::string> queue_sensor;
	boost::optional<int> area;
};

struct actuator_data {
	std::string name;
	int exits;

	std::vector<detailed_data> before;
	std::vector<detailed_data> after;

	std::set<std::string> dependent;
	//if this actuator gets green all from dependent set should get green
	std::map<std::string, int> collides;
	//if this actuator gets green none from collides map should get green
	// green should change to red with given in value cooldown
};

struct controlled_area_data {
	int scope;
	std::map<std::string, actuator_data> actuators;
};

class controlled_area {
public:
	controlled_area(const std::string& filename_);
	virtual ~controlled_area();

	bool parse();

private:
	bool parse_area(const std::string& key, const boost::property_tree::ptree& pt);

	bool parse_actuators(const boost::property_tree::ptree& pt);
	bool parse_actuator(const boost::property_tree::ptree& pt);
	bool parse_dependencies(const boost::property_tree::ptree& pt);
	bool parse_collisions(const boost::property_tree::ptree& pt);

	common::my_logger logger;
	std::string filename;

	controlled_area_data data;
};

} // namespace controler

#endif /* CONTROLLED_AREA_H */
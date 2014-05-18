#include "algorithm.h"

#include <boost/foreach.hpp>
#include <boost/assign.hpp>

namespace controller {

algorithm::algorithm(const std::string& name, set_actuator_cb_type cb, controlled_area_data area_) :
	logger(name),
	set_actuator_cb(cb),
	area(area_)
{
	typedef std::pair<std::string, actuator_data> act_data_pair;
	typedef std::pair<std::string, int> str_int_pair;

	BOOST_FOREACH(act_data_pair pair, area.actuators) {
		objects.insert(std::make_pair(
			pair.first, algorithm_object_handle(new algorithm_object(pair.first))));
	}

	BOOST_FOREACH(act_data_pair pair, area.actuators) {
		BOOST_FOREACH(std::string str, pair.second.dependent) {
			objects[pair.first]->add_dependant(objects[str]);
		}

		BOOST_FOREACH(str_int_pair str, pair.second.collides) {
			objects[pair.first]->add_collision(objects[str.first], str.second);
		}
	}
}

algorithm::~algorithm()
{}

std::string algorithm::as_short_string(EActuatorState state)
{
	static std::map<EActuatorState, std::string> map = boost::assign::map_list_of
		(EActuatorState_Off, " - ")(EActuatorState_Green, " G ")
		(EActuatorState_Yellow, " Y ")(EActuatorState_Red, " R ")
		(EActuatorState_RedYellow, "R-Y");

	return map.find(state)->second;
}


} // namespace controller
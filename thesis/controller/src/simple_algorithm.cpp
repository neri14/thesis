#include "simple_algorithm.h"

#include <config/config.h>

#include <boost/assign.hpp>
#include <boost/foreach.hpp>
#include <boost/property_tree/xml_parser.hpp>

namespace controller {

simple_algorithm::simple_algorithm(set_actuator_cb_type cb, controlled_area_data area_) :
	algorithm("simple_algorithm", cb, area_),
	cycle_time(common::get_config().get<int>("cycle_time"))
{
	parse_algorithm_data();

	typedef std::pair<std::string, actuator_data> actuator_type;
	BOOST_FOREACH(actuator_type act, area.actuators) {
		current_state.insert(std::make_pair(act.first, EActuatorState_Off));
	}
}

simple_algorithm::~simple_algorithm()
{}

void simple_algorithm::on_queue_sensor_update(const std::string& name, int time_tick, int queue, int max_queue)
{}

void simple_algorithm::on_flow_sensor_update(const std::string& name, int timt_tick, int flow)
{}

void simple_algorithm::on_time_tick(int time_tick)
{
	static int token = 0;

	typedef std::pair<std::string, actuator_data> actuator_type;
	BOOST_FOREACH(actuator_type act, area.actuators) {
		int t = time_tick%cycle_time;
		int green_start = 0;
		int green_stop = 0;

		BOOST_FOREACH(simple_algorithm_state st, states) {
			if (st.actuators.end() != st.actuators.find(act.first)) {
				green_stop = green_start + static_cast<int>((st.time/100.0)*cycle_time);
				if (green_stop >= cycle_time) {
					green_stop = cycle_time-1;
				}
				break;
			}
			green_start += static_cast<int>((st.time/100.0)*cycle_time);

			if (green_start >= cycle_time) {
				logger.warning()() << "unreachable state";
				green_start = cycle_time-1;
			}
		}

		objects[act.first]->set_state(t >= green_start && t < green_stop, ++token);
	}

	typedef std::pair<std::string, algorithm_object_handle> alg_obj_type;
	BOOST_FOREACH(alg_obj_type pair, objects) {
		pair.second->notif_time_tick(time_tick);
	}
	BOOST_FOREACH(alg_obj_type pair, objects) {
		algorithm_object_handle o = pair.second;
		if (o->has_state_changed()) {
			logger.info()() << "[" << time_tick << "] " << o->get_name() <<
				" changing state to " << as_string(o->get_state());
			set_actuator_cb(o->get_name(), o->get_state());
		}
	}
}

void simple_algorithm::parse_algorithm_data()
{
	std::string filename = common::get_config().get<std::string>("simple_alogrithm_data");
	logger.info()() << "parsing simple algorithm data: " << filename;

	try {
		using boost::property_tree::ptree;
		ptree pt;

		read_xml(filename, pt);

		BOOST_FOREACH(const ptree::value_type& v_state, pt.get_child("simple_algorithm")) {
			simple_algorithm_state state;
			state.time = v_state.second.get<int>("time");

			if ("state" == v_state.first) {
				BOOST_FOREACH(const ptree::value_type& v_act, v_state.second.get_child("actuators")) {
					if ("name" == v_act.first) {
						state.actuators.insert(v_act.second.get_value<std::string>());
					}
				}
			}

			states.push_back(state);
			logger.debug()() << "added state with " << state.actuators.size() << " actuators for " <<
				state.time << " / 100 of time";
		}
	} catch (std::exception& e) {
		logger.error()() << "parsing error: " << e.what();
	}

	logger.info()() << "parsed simple algorithm data: " << filename;
}

std::string simple_algorithm::as_string(EActuatorState state)
{
	static std::map<EActuatorState, std::string> map = boost::assign::map_list_of
		(EActuatorState_Off, "OFF")(EActuatorState_Green, "GREEN")
		(EActuatorState_Yellow, "YELLOW")(EActuatorState_Red, "RED")
		(EActuatorState_RedYellow, "RED-YELLOW");

	return map.find(state)->second;
}

} // namespace controller
#include "dynamic_algorithm.h"

#include <config/config.h>
#include <boost/foreach.hpp>
#include <boost/assign.hpp>

#include <dispatcher/event/event.h>
#include <dispatcher/event/payload/expected_flow_payload.h>

namespace controller {

dynamic_algorithm::dynamic_algorithm(set_actuator_cb_type cb, controlled_area_data area_) :
	algorithm("dynamic_algorithm", cb, area_),
	cycle_time(common::get_config().get<int>("cycle_time")),
	expected_flow_factor(common::get_config().get<double>("expected_flow_factor")),
	current_flow_factor(common::get_config().get<double>("current_flow_factor")),
	current_queue_factor(common::get_config().get<double>("current_queue_factor")),
	max_flow(common::get_config().get<int>("max_flow"))
{
	expected_flow_listener = common::dispatcher::get_dispatcher().register_listener(
		EEventType_ExpectedFlow, static_cast<EEventScope>(area_.scope),
		boost::bind(&dynamic_algorithm::on_expected_flow_update, this, _1));

	prepare_possible_states();
}

dynamic_algorithm::~dynamic_algorithm()
{}

void dynamic_algorithm::on_queue_sensor_update(const std::string& name, int time_tick, int queue, int max_queue_)
{
	static int last_tick = 0;
	if (last_tick < time_tick) {
		last_tick = time_tick;

		typedef std::pair<std::string, int> str_int_pair;
		BOOST_FOREACH(str_int_pair pair, current_queue) {
			pair.second = 0;
		}
		BOOST_FOREACH(str_int_pair pair, max_queue) {
			pair.second = 0;
		}
	}

	std::set<std::string> actuators;

	typedef std::pair<std::string, actuator_data> str_actuator_pair;
	BOOST_FOREACH(str_actuator_pair act, area.actuators) {
		BOOST_FOREACH (detailed_data dd, act.second.before) {
			if (dd.queue_sensors.find(name) != dd.queue_sensors.end()) {
				actuators.insert(act.first);
			}
		}
	}

	BOOST_FOREACH(std::string act, actuators) {
		current_queue[act] += queue;
		max_queue[act] += max_queue_;
		logger.debug()() << "actutator " << act << " queue is " << current_queue[act];
	}
}

void dynamic_algorithm::dispatch_expected_flow_events(int tick)
{
	std::map<std::string, std::set<int> > after_areas;
	std::map<int, int> area_flow_map;

	typedef std::pair<std::string, actuator_data> str_actuator_pair;
	BOOST_FOREACH(str_actuator_pair act, area.actuators) {
		after_areas[act.first] = std::set<int>();

		BOOST_FOREACH (detailed_data dd, act.second.after) {
			if (dd.area) {
				after_areas[act.first].insert(dd.area.get());
				area_flow_map[dd.area.get()] = 0;
			}
		}
	}

	typedef std::pair<std::string, std::set<int> > str_intset_pair;
	BOOST_FOREACH(str_intset_pair pair, after_areas) {
		if (!pair.second.empty() &&
				EActuatorState_Green == objects[pair.first]->get_state()) {
			int exp_flow = max_flow / area.actuators[pair.first].exits;

			BOOST_FOREACH(int area, pair.second) {
				area_flow_map[area] += exp_flow;
			}
		}
	}

	typedef std::pair<int, int> int_int_pair;
	BOOST_FOREACH(int_int_pair pair, area_flow_map) {
		EEventScope sc = static_cast<EEventScope>(pair.first);

		logger.debug()() << "expected flow to area " << pair.first << " is " << pair.second;
		common::dispatcher::payload_handle payload(
			new common::dispatcher::expected_flow_payload(area.scope, pair.second, tick));
		common::dispatcher::get_dispatcher().dispatch(common::dispatcher::event_handle(
			new common::dispatcher::event(EEventType_ExpectedFlow, sc, payload)));
	}
}

void dynamic_algorithm::on_flow_sensor_update(const std::string& name, int time_tick, int flow)
{
	std::set<std::string> actuators;

	typedef std::pair<std::string, actuator_data> str_actuator_pair;
	BOOST_FOREACH(str_actuator_pair act, area.actuators) {
		BOOST_FOREACH (detailed_data dd, act.second.before) {
			if (dd.flow_sensor == name) {
				actuators.insert(act.first);
			}
		}
	}

	BOOST_FOREACH(std::string act, actuators) {
		current_flow[act] = flow;
	}
}

void dynamic_algorithm::on_expected_flow_update(common::dispatcher::event_handle ev)
{
	BOOST_ASSERT(EEventType_ExpectedFlow == ev->get_type());
	BOOST_ASSERT(static_cast<EEventScope>(area.scope) == ev->get_scope());
	common::dispatcher::expected_flow_payload& payload =
		*ev->get_payload<common::dispatcher::expected_flow_payload>();

	logger.debug()() << "received expected flow from area " << payload.origin_area;

	std::set<std::string> act_names;
	typedef std::pair<std::string, actuator_data> str_actuator_pair;
	BOOST_FOREACH(str_actuator_pair act, area.actuators) {
		BOOST_FOREACH (detailed_data before, act.second.before) {
			if (before.area) {
				expected_flow[act.first] = 0;
				act_names.insert(act.first);
			}
		}
	}

	int flow = payload.flow/act_names.size();
	BOOST_FOREACH(std::string str, act_names) {
		logger.debug()() << "expected flow to actuator " << str << " is " << flow;
		expected_flow[str] = flow;
	}
}

void dynamic_algorithm::on_time_tick(int time_tick)
{
	static int token = 0;

	bool during_state_change = false;
	typedef std::pair<std::string, algorithm_object_handle> str_algorithm_object_pair;
	BOOST_FOREACH(str_algorithm_object_pair pair, objects) {
		during_state_change = during_state_change || pair.second->is_during_state_change();
	}

	typedef std::pair<std::string, algorithm_object_handle> alg_obj_type;
	if (!during_state_change) {
		penelty_weight.clear();
		BOOST_FOREACH(alg_obj_type pair, objects) {
			if (EActuatorState_Red == pair.second->get_state() &&
					pair.second->get_current_state_timer() >= cycle_time) {
				penelty_weight.insert(pair.first);
			}
		}

		std::set<std::string> new_states = calculate_new_states();

		BOOST_FOREACH(str_algorithm_object_pair pair, objects) {
			if (new_states.find(pair.first) == new_states.end()) {
				pair.second->set_state(false, ++token);
			} else {
				pair.second->set_state(true, ++token);
			}
		}
	}

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

	{
		common::my_logger_stream str = logger.debug();
		str() << "[" << time_tick << "] current states are: ";

		BOOST_FOREACH(alg_obj_type pair, objects) {
			algorithm_object_handle o = pair.second;
			str() << as_short_string(o->get_state()) << " ";
		}
	}

	dispatch_expected_flow_events(time_tick);
}

std::set<std::string> dynamic_algorithm::calculate_new_states()
{
	//1. calculate weight of every actuator
	std::map<std::string, double> weight_map;

	typedef std::pair<std::string, actuator_data> str_actuator_data_pair;
	BOOST_FOREACH(str_actuator_data_pair act, area.actuators) {
		double expected_flow_val = 0;
		double current_flow_val = 0;
		double current_queue_val = 0;

		if (expected_flow.find(act.first) != expected_flow.end()) {
			expected_flow_val = static_cast<double>(expected_flow[act.first]) / static_cast<double>(max_flow);
			logger.debug()() << "weight expected_flow_val=" << expected_flow_val;
		}
		if (current_flow.find(act.first) != current_flow.end()) {
			current_flow_val = static_cast<double>(current_flow[act.first]) / static_cast<double>(max_flow);
			logger.debug()() << "weight current_flow_val=" << current_flow_val;
		}
		if (current_queue.find(act.first) != current_queue.end() &&
				max_queue.find(act.first) != max_queue.end()) {
			current_queue_val = static_cast<double>(current_queue[act.first]) / static_cast<double>(max_queue[act.first]);
			logger.debug()() << "weight current_queue_val=" << current_queue_val;
		}

		double weight = expected_flow_factor*expected_flow_val +
			current_flow_factor*current_flow_val + current_queue_factor*current_queue_val;
		weight_map[act.first] = weight;
		logger.debug()() << "actuator " << act.first << " weight is " << weight;
	}

	//1b. add override weight to overdue changes
	BOOST_FOREACH(std::string s, penelty_weight) {
		weight_map[s] += 1;
	}

	//2. calculate summary weight of every state
	typedef std::set<std::string> str_bool_set;

	std::set<std::string> hi_weight_set = *possible_states.begin();
	double hi_weight = calculate_weight(hi_weight_set, weight_map);

	BOOST_FOREACH(str_bool_set set, possible_states) {
		double w = calculate_weight(set, weight_map);
		if (w > hi_weight) {
			hi_weight = w;
			hi_weight_set = set;
		} else if (w == hi_weight && hi_weight_set.size() > set.size()) {
			hi_weight = w;
			hi_weight_set = set;
		}
	}

	//3. choose heighest weight state and return
	logger.debug()() << "heighest weight " << hi_weight << " with " << hi_weight_set.size() << " actuators";
	return hi_weight_set;
}

double dynamic_algorithm::calculate_weight(std::set<std::string> state_set, std::map<std::string, double> weight_map)
{
	double weight = 0;

	BOOST_FOREACH(std::string state, state_set) {
		if (weight_map.find(state) != weight_map.end()) {
			weight += weight_map[state];
		}
	}

	return weight;
}

std::string dynamic_algorithm::as_string(EActuatorState state)
{
	static std::map<EActuatorState, std::string> map = boost::assign::map_list_of
		(EActuatorState_Off, "OFF")(EActuatorState_Green, "GREEN")
		(EActuatorState_Yellow, "YELLOW")(EActuatorState_Red, "RED")
		(EActuatorState_RedYellow, "RED-YELLOW");

	return map.find(state)->second;
}

void dynamic_algorithm::prepare_possible_states()
{
	std::set<std::string> state;
	possible_states.insert(state);

	typedef std::pair<std::string, actuator_data> str_actuator_data_pair;
	BOOST_FOREACH(str_actuator_data_pair act, area.actuators) {
		create_possible_states(state, act.first);
	}

	logger.info()() << "prepared " << possible_states.size() << " possible states";
}

void dynamic_algorithm::create_possible_states(std::set<std::string> state, std::string actuator)
{
	if (state.find(actuator) != state.end()) {
		return;
	}

	typedef std::pair<std::string, int> str_int_pair;
	BOOST_FOREACH(str_int_pair collision, area.actuators[actuator].collides) {
		if (state.find(collision.first) != state.end()) {
			return;
		}
	}

	state.insert(actuator);

	if (possible_states.find(state) == possible_states.end()) {
		possible_states.insert(state);

		{
			common::my_logger_stream str = logger.debug();
			str() << "created possible state with " << state.size() << " actuators green: ";
			BOOST_FOREACH(std::string s, state) {
				str() << s << " ";
			}
		}
	}

	typedef std::pair<std::string, actuator_data> str_actuator_data_pair;
	BOOST_FOREACH(str_actuator_data_pair act, area.actuators) {
		create_possible_states(state, act.first);
	}
}

}
#include "simulation.h"

#include <csv_writer.h>

#include "../world/world_description.h"
#include "../world/world_description_parser.h"
#include "../world/world_description_validator.h"

#include <config/config.h>
#include <dispatcher/event/payload/simulation_state_calculated.h>
#include <dispatcher/event/payload/time_tick_payload.h>
#include <dispatcher/event/payload/actuator_finished_payload.h>
#include <dispatcher/event/payload/queue_sensor_state.h>
#include <dispatcher/event/payload/flow_sensor_state.h>

#include <cmath>

#include <boost/foreach.hpp>
#include <boost/bind.hpp>

namespace simulator {
namespace simulation {

namespace constant {
	int safety_cell_count_limit(10000);
	int safety_multiplier_sim(3);
	std::string csv_key("average_speed_m_s");
	std::string csv_key2("vehicle_count");

	int stuck_vehicle_removal_time(10);
}

simulation::simulation(const std::string& desc_filename_) :
	logger("simulation"),
	desc_filename(desc_filename_),
	simulation_duration(0),
	max_speed(0),
	pending_time_tick(-1),
	last_calculated_tick(-1)
{
	common::get_csv_writer().add_key(constant::csv_key);
	common::get_csv_writer().add_key(constant::csv_key2);
}

simulation::~simulation()
{
	BOOST_FOREACH(common::dispatcher::connection_handle listener, listeners) {
		common::dispatcher::get_dispatcher().unregister_listener(listener);
	}
}

bool simulation::prepare()
{
	logger.info()() << "parsing world description file";
	world::world_description_parser parser(common::get_config().get<std::string>("world_description_file"));
	if (!parser.parse()) {
		return false;
	}

	logger.info()() << "validating world description";
	world::world_description_validator validator(parser.get_world_description());
	if (!validator.validate()) {
		return false;
	}

	logger.info()() << "translating graph representation of world description to cellular representation";
	if (!translate_to_cell_representation(parser.get_world_description())) {
		return false;
	}

	logger.debug()() << "preparing identifiers set";
	prepare_identifiers();

	logger.info()() << "registering for events";
	listeners.insert(common::dispatcher::get_dispatcher().register_listener(
		EEventType_TimeTick, EEventScope_General,
		boost::bind(&simulation::on_time_tick, this, _1)));
	listeners.insert(common::dispatcher::get_dispatcher().register_listener(
		EEventType_ActuatorFinished, EEventScope_Local,
		boost::bind(&simulation::on_actuator_finished, this, _1)));
	listeners.insert(common::dispatcher::get_dispatcher().register_listener(
		EEventType_QueueSensorState, EEventScope_Any,
		boost::bind(&simulation::on_queue_sensor_state, this, _1)));
	listeners.insert(common::dispatcher::get_dispatcher().register_listener(
		EEventType_FlowSensorState, EEventScope_Any,
		boost::bind(&simulation::on_flow_sensor_state, this, _1)));

	return true;
}

int simulation::get_duration() const
{
	return simulation_duration;
}

void simulation::on_time_tick(common::dispatcher::event_handle ev)
{
	BOOST_ASSERT(EEventType_TimeTick == ev->get_type());
	BOOST_ASSERT(EEventScope_General == ev->get_scope());

	pending_time_tick = ev->get_payload<common::dispatcher::time_tick_payload>()->tick;
	pending_set.clear();
	pending_set.insert(identifiers.begin(), identifiers.end());

	check_calculate_condition();
}

void simulation::on_actuator_finished(common::dispatcher::event_handle ev)
{
	BOOST_ASSERT(EEventType_ActuatorFinished == ev->get_type());
	common::dispatcher::actuator_finished_payload& act =
		*ev->get_payload<common::dispatcher::actuator_finished_payload>();
	clear_pending(act.time_tick, act.get_actuator_name());

	check_calculate_condition();
}

void simulation::on_queue_sensor_state(common::dispatcher::event_handle ev)
{
	BOOST_ASSERT(EEventType_QueueSensorState == ev->get_type());
	common::dispatcher::queue_sensor_state& sensor =
		*ev->get_payload<common::dispatcher::queue_sensor_state>();
	clear_pending(sensor.time_tick, sensor.get_sensor_name());

	check_calculate_condition();
}

void simulation::on_flow_sensor_state(common::dispatcher::event_handle ev)
{
	BOOST_ASSERT(EEventType_FlowSensorState == ev->get_type());
	common::dispatcher::flow_sensor_state& sensor =
		*ev->get_payload<common::dispatcher::flow_sensor_state>();
	clear_pending(sensor.time_tick, sensor.get_sensor_name());

	check_calculate_condition();
}

void simulation::clear_pending(int time_tick, std::string identifier)
{
	if (pending_time_tick == time_tick) {
		if (pending_set.find(identifier) != pending_set.end()) {
			pending_set.erase(identifier);
		} else {
			logger.warning()() << "received unknown component finished: " << identifier;
		}
	} else {
		logger.warning()() << "component (" << identifier << ") finished in wrong tick: current=" <<
			pending_time_tick << " received=" << time_tick;
	}
}

void simulation::check_calculate_condition()
{
	if (last_calculated_tick != pending_time_tick && pending_set.empty()) {
		run_creators(pending_time_tick);
		run_destroyers(pending_time_tick);

		calculate_new_vehicles_state();

		common::dispatcher::payload_handle payload(
			new common::dispatcher::simulation_state_calculated(pending_time_tick));
		common::dispatcher::get_dispatcher().dispatch(common::dispatcher::event_handle(
			new common::dispatcher::event(EEventType_SimulationStateCalculated,
				EEventScope_Local, payload)));

		last_calculated_tick = pending_time_tick;
	}
}

void simulation::run_creators(int time_tick)
{
	logger.info()() << "creating vehicles";
	int count = 0;
	int waiting = 0;

	typedef std::pair<cell_handle, creator_handle> cell_creator_pair;
	BOOST_FOREACH(cell_creator_pair pair, creators) {
		vehicle_handle v = pair.second->create(pending_time_tick);
		if (v) {
			pair.first->set_occupied(true, v->get_speed());
			vehicles.insert(v);
			++count;
		}

		if (pair.second->get_queue_size()) {
			std::string ent_name;

			typedef std::pair<std::string, cell_handle> str_cell_type;
			BOOST_FOREACH(str_cell_type sc, cell_names) {
				if (sc.second == pair.first) {
					ent_name = sc.first;
				}
			}

			logger.warning()() << pair.second->get_queue_size() << " of waiting vehicles on entrance " <<
				ent_name;
		}
		waiting += pair.second->get_queue_size();
	}

	logger.info()() << "created " << count << " vehicles - " << waiting << " waiting";
}

void simulation::run_destroyers(int time_tick)
{
	static int rolling = 0;

	logger.info()() << "destroying vehicles";
	int veh_count = 0;
	int cell_count = 0;

	typedef std::pair<cell_handle, destroyer_handle> cell_destroyer_pair;
	BOOST_FOREACH(cell_destroyer_pair cell_dest, destroyers) {
		if (cell_dest.second->should_destroy(time_tick)) {
			cell_dest.first->set_occupied(false);

			std::set<vehicle_handle> vehicles_to_destroy;
			BOOST_FOREACH(vehicle_handle veh, vehicles) {
				if (veh->get_path().front().cell_h == cell_dest.first) {
					veh->set_stop_tick(time_tick);
					vehicles_to_destroy.insert(veh);
				}
			}
			BOOST_FOREACH(vehicle_handle v, vehicles_to_destroy) {
				vehicles.erase(v);
				++veh_count;
			}
			++cell_count;
		}
	}

	rolling += veh_count;
	logger.info()() << "destroyed " << veh_count << " vehicles in " <<
		cell_count << " cells (rolling sum: " << rolling << ")";

	/*if (veh_count != cell_count) {
		logger.warning()() << "multiple vehicles on cell detected when destroying";
	}*/
}

std::string get_next_node_name(std::queue<path_cell> queue, std::map<std::string, cell_handle> cell_names)
{
	typedef std::pair<std::string, cell_handle> str_cell_pair;
	while (!queue.empty()) {
		BOOST_FOREACH(str_cell_pair p, cell_names) {
			if (queue.front().cell_h == p.second) {
				return p.first;
			}
		}
		queue.pop();
	}
	return "";
}

//model Nagela-Schreckenberga
void simulation::calculate_new_vehicles_state()
{
	logger.info()() << "calculating new vehicles states";

	int cells_size = cells.size();
	double avg = 0;

	std::set<vehicle_handle> veh_to_remove;
	BOOST_FOREACH(vehicle_handle veh, vehicles) {
		int speed = veh->get_speed();
		std::queue<path_cell>& p_cells = veh->get_path();

		//SPEEDUP
		if (speed < max_speed) {
			++speed;
		}

		//SLOWDOWN
		int gap = calculate_gap(veh, p_cells);
		if (speed > gap) {
			speed = gap;
		}

		veh->set_speed(speed);
		avg += speed;

		//MOVE
		p_cells.front().cell_h->set_occupied(false);
		for (int i=0; i<speed; ++i) {
			p_cells.front().cell_h->increment_vehicle_counter();
			p_cells.pop();
		}
		p_cells.front().cell_h->set_occupied(true, speed);

		if (speed == 0 && p_cells.front().cell_h->get_entrances_count() > 1) {
			logger.error()() << "vehicle stopped on multientrance cell for " << veh->get_current_speed_time() << " seconds (" << get_next_node_name(p_cells, cell_names) << ")";

			if (veh->get_current_speed_time() > constant::stuck_vehicle_removal_time) {
				logger.error()() << "removing stuck vehicle (stopped for over " << constant::stuck_vehicle_removal_time << " seconds)";
				p_cells.front().cell_h->set_occupied(false);
				veh_to_remove.insert(veh);
			}
		}
	}

	int to_remove_size = veh_to_remove.size();
	if (to_remove_size) {
		logger.error()() << to_remove_size << " stuck vehicles being removed";
	}
	BOOST_FOREACH(vehicle_handle veh, veh_to_remove) {
		vehicles.erase(veh);
	}

	int veh_cnt = vehicles.size();
	avg = veh_cnt ? avg/veh_cnt : 0;

	common::get_csv_writer().add_value(pending_time_tick, constant::csv_key, (int)(avg*7.5));
	common::get_csv_writer().add_value(pending_time_tick, constant::csv_key2, veh_cnt);
	logger.info()() << "average speed of " << veh_cnt << " vehicles is " << avg << " (number of cells " << cells_size << ")";
}

int simulation::calculate_gap(vehicle_handle veh, std::queue<path_cell> p_cells)
{
	if (!p_cells.empty() && !p_cells.front().cell_h->is_exit_allowed(p_cells.front().exit)) {
		return 0;
	}

	bool vehicle_curently_on_multientrance = p_cells.front().cell_h->get_entrances_count() > 1;

	int gap = 0;
	p_cells.pop();

	bool can_proceed = true;
	while(!p_cells.empty() && can_proceed && gap < max_speed) {
		cell_handle cell_h = p_cells.front().cell_h;

		bool is_cell_multientrance = cell_h->get_entrances_count() > 1;

		if (cell_h->is_occupied()) {
			can_proceed = false;
		} else if (is_cell_multientrance && !can_enter_multientrance(p_cells, vehicle_curently_on_multientrance, gap)) {
			can_proceed = false;
		} else {
			can_proceed = cell_h->is_exit_allowed(p_cells.front().exit);
			p_cells.pop();
			++gap;
		}
	}

	return gap;
}

bool simulation::can_enter_multientrance(std::queue<path_cell> p_cells, bool vehicle_curently_on_multientrance, int gap)
{
	int cnt = 0;

	while (!p_cells.empty()) {
		bool is_multientrance = p_cells.front().cell_h->get_entrances_count() > 1;

		if (is_multientrance && p_cells.front().cell_h->is_occupied()) {
			// if (vehicle_curently_on_multientrance) {
			// 	logger.error()() << "VEH currently on multientrance and stopping (gap to first multientrance " << gap
			// 		<< " , multientrance in row " << cnt << ") reason OCCUPIED_MULTIENTRANCE";
			// }
			return false;
		} else if (!is_multientrance && p_cells.front().cell_h->is_occupied()) {
			// if (vehicle_curently_on_multientrance) {
			// 	logger.error()() << "VEH currently on multientrance and stopping (gap to first multientrance " << gap
			// 		<< " , multientrance in row " << cnt << ") reason OCCUPIED_NON_MULTIENTRANCE";
			// }
			return false;
		} else if (!is_multientrance) {
			return true;
		} else if (!vehicle_curently_on_multientrance &&
				p_cells.front().cell_h->get_priority_entrance_number() != p_cells.front().entrance &&
				!can_non_priority_enter(p_cells.front().cell_h)) {
			// if (vehicle_curently_on_multientrance) {
			// 	logger.error()() << "VEH currently on multientrance and stopping (gap to first multientrance " << gap
			// 		<< " , multientrance in row " << cnt << ") reason NON_PRIORITY_MULTIENTRANCE";
			// }
			return false;
		}

		p_cells.pop();
		++cnt;
	}
	return true;
}

bool simulation::can_non_priority_enter(cell_handle cell_h)
{
	static int safety_margin = max_speed*constant::safety_multiplier_sim;
	cell_handle prev = cell_h->get_prev(cell_h->get_priority_entrance_number()).lock();

	return !prev->prev_vehicle_moving(safety_margin, cell_h.get());
}

bool simulation::translate_to_cell_representation(world::world_description_handle desc)
{
	try {
		logger.info()() << "translating to cell representation";
		if (!translate_nodes(desc)) {
			logger.error()() << "translating nodes failed";
			return false;
		}

		if (!translate_actuators(desc)) {
			logger.error()() << "translating actuators failed";
			return false;
		}

		if (!translate_flow_sensors(desc)) {
			logger.error()() << "translating flow sensors failed";
			return false;
		}

		if (!translate_queue_sensors(desc)) {
			logger.error()() << "translating queue sensors failed";
			return false;
		}

		if (!translate_paths(desc)) {
			logger.error()() << "translating paths failed";
			return false;
		}

		if (!translate_simulation_data(desc)) {
			logger.error()() << "translating simulation data failed";
			return false;
		}

		logger.info()() << "translated to cell representation";
		return true;
	} catch (std::exception& e) {
		logger.error()() << "unexpected exception: " << e.what();
		return false;
	}
}

bool simulation::translate_nodes(world::world_description_handle desc)
{
	typedef std::pair<std::string, world::world_node_handle> node_pair_t;
	//create cells based on nodes (+ add them to cell_names map)
	BOOST_FOREACH(node_pair_t node, desc->nodes) {
		cell_handle c(new cell(node.second->priority_entrance));

		if (node.second->max_create_rate) {
			creator_handle tmp(new creator(node.first, c, node.second->max_create_rate, desc->simulation->max_speed));
			creators.insert(std::make_pair(c, tmp));
		}
		if (node.second->max_destroy_rate) {
			destroyer_handle tmp(new destroyer(c, node.second->max_destroy_rate));
			destroyers.insert(std::make_pair(c, tmp));
			c->enable_destroyer_hack();
		}

		cells.insert(c);
		cell_names.insert(std::make_pair(node.first, c));

		logger.debug()() << "created cell for node " << node.first;
	}
	logger.debug()() << "created cells for " << cells.size() << " nodes";
	logger.debug()() << "created " << creators.size() << " creators";
	logger.debug()() << "created " << destroyers.size() << " destroyers";

	//create cells based on connection (betweent previous ones)
	BOOST_FOREACH (world::world_connection_handle conn, desc->connections) {
		world::world_node_handle node_from = conn->from.lock();
		world::world_node_handle node_to = conn->to.lock();

		cell_handle cell_from = cell_names.find(node_from->name)->second;
		cell_handle cell_to = cell_names.find(node_to->name)->second;

		int cells_count = ceil(conn->distance/desc->simulation->cell_size);

		std::pair<int, int> ends_pair = node_from->find_connection_to(*node_to);

		cell_handle last_cell = cell_from;
		int exit_num = ends_pair.first;
		int entr_num = 0;
		for (int i=0; i<cells_count; ++i) {
			cell_handle c(new cell());
			cells.insert(c);

			last_cell->add_next(exit_num, c);
			c->add_prev(entr_num, last_cell);

			last_cell = c;
			if (exit_num) {
				exit_num = 0;
			}
		}
		entr_num = ends_pair.second;

		last_cell->add_next(exit_num, cell_to);
		cell_to->add_prev(entr_num, last_cell);

		logger.debug()() << "created " << cells_count << " cells from " <<
			node_from->name << " to " << node_to->name;

		//check if cells are correct
		int counted = 0;
		last_cell = cell_from;
		for (cell_handle curr = cell_from->get_next(ends_pair.first).lock();
				curr && curr != cell_to; curr = curr->get_next(0).lock()) {
			BOOST_ASSERT(last_cell == curr->get_prev(0).lock());
			if (last_cell != curr->get_prev(0).lock()) {
				logger.error()() << "cells are not connected correctly";
				return false;
			}
			++counted;
			last_cell = curr;
		}
		if (cells_count != counted) {
			logger.error()() << "wrong number of cells created, expected: " <<
				cells_count << " there is: " << counted;
			return false;
		}
	}

	return true;
}

bool simulation::translate_actuators(world::world_description_handle desc)
{
	typedef std::pair<std::string, world::world_area_handle> area_pair_t;

	BOOST_FOREACH (area_pair_t ar, desc->areas) {
		int act_count = 0;
		BOOST_FOREACH (world::world_actuator_handle act, ar.second->actuators) {
			if (cell_names.end() == cell_names.find(act->node->name)) {
				logger.error()() << "cell of given name doesn't exist (" << act->node->name << ")";
				return false;
			}

			cell_handle c = cell_names.find(act->node->name)->second;
			actuator_handle tmp(new actuator(ar.second->name,
				static_cast<EEventScope>(ar.second->scope),
				act->name, c, act->exit));
			actuators.insert(tmp);

			++act_count;
		}
		logger.debug()() << "created " << act_count << " actuators in area " << ar.first;
	}
	return true;
}

bool simulation::translate_flow_sensors(world::world_description_handle desc)
{
	typedef std::pair<std::string, world::world_area_handle> area_pair_t;

	BOOST_FOREACH (area_pair_t ar, desc->areas) {
		int sensor_count = 0;
		BOOST_FOREACH (world::world_flow_sensor_handle sens, ar.second->flow_sensors) {
			if (cell_names.end() == cell_names.find(sens->node->name)) {
				logger.error()() << "cell of given name doesn't exist (" << sens->node->name << ")";
				return false;
			}

		 	cell_handle c = cell_names.find(sens->node->name)->second;
		 	flow_sensor_handle tmp(new flow_sensor(ar.second->name,
		 		static_cast<EEventScope>(ar.second->scope), sens->name, c));
		 	flow_sensors.insert(tmp);

		 	++sensor_count;
		}
		logger.debug()() << "created " << sensor_count << " flow sensors in area " << ar.first;
	}
	return true;
}

bool simulation::translate_queue_sensors(world::world_description_handle desc)
{
	typedef std::pair<std::string, world::world_area_handle> area_pair_t;

	BOOST_FOREACH (area_pair_t ar, desc->areas) {
		int sensor_count = 0;
		BOOST_FOREACH (world::world_queue_sensor_handle sens, ar.second->queue_sensors) {
			if (cell_names.end() == cell_names.find(sens->node_from->name)) {
				logger.error()() << "cell of given name doesn't exist (" << sens->node_from->name << ")";
				return false;
			}
			if (cell_names.end() == cell_names.find(sens->node_to->name)) {
				logger.error()() << "cell of given name doesn't exist (" << sens->node_to->name << ")";
				return false;
			}

		 	cell_handle c_from = cell_names.find(sens->node_from->name)->second;
		 	cell_handle c_to = cell_names.find(sens->node_to->name)->second;

		 	int c_from_exit = sens->node_from_exit;
		 	int c_to_entrance = sens->node_to_entrance;

		 	queue_sensor_handle tmp(new queue_sensor(ar.second->name,
		 		static_cast<EEventScope>(ar.second->scope), sens->name, c_from, c_to,
		 		c_from_exit, c_to_entrance));
		 	queue_sensors.insert(tmp);

		 	++sensor_count;
		}
		logger.debug()() << "created " << sensor_count << " queue sensors in area " << ar.first;
	}
	return true;
}

bool simulation::translate_paths(world::world_description_handle desc)
{
	typedef std::pair<std::string, world::world_path_handle> path_pair_t;

	BOOST_FOREACH(path_pair_t pth, desc->paths){
		if (pth.second->nodes.empty()) {
			logger.error()() << "can't translate empty path";
			return false;
		}
		if (creators.end() == creators.find(cell_names[(*pth.second->nodes.begin())->name])) {
			logger.error()() << "creator for given cell was not created";
			return false;
		}

		path_handle tmp(new path(pth.first));

		world::world_node_handle prev_nde;
		path_cell* last_added = 0;
		BOOST_FOREACH(world::world_node_handle nde, pth.second->nodes) {
			if (prev_nde) {
				cell_handle c_from = cell_names[prev_nde->name];
				cell_handle c_to = cell_names[nde->name];
				std::pair<int, int> ex_ent = prev_nde->find_connection_to(*nde);

				if (last_added) {
				 	last_added->exit = ex_ent.first;
				} else {
					last_added = tmp->add_cell(c_from, 0, ex_ent.first);
				}

				int cnt = constant::safety_cell_count_limit;
				int nde_cnt = 0;
				cell_handle c = c_from->get_next(ex_ent.first).lock();
				while (cnt && c != c_to) {
					last_added = tmp->add_cell(c, 0, 0);
					c = c->get_next(0).lock();
					++nde_cnt;
					--cnt;
				}

				if (!cnt) {
					logger.error()() << "no path between cells";
					return false;
				}

				last_added = tmp->add_cell(c_to, ex_ent.second, 0);
			}
			prev_nde = nde;
		}

		paths.insert(tmp);
		creators[cell_names[(*pth.second->nodes.begin())->name]]->add_path(tmp);
		logger.debug()() << "created path " << pth.second->name <<
			" throught " << tmp->get_cells().size() << " cells";
	}
	return true;
}

bool simulation::translate_simulation_data(world::world_description_handle desc)
{
	simulation_duration = desc->simulation->duration;
	max_speed = desc->simulation->max_speed;

	typedef std::pair<int, std::pair<int, world::world_path_handle> > path_flow_type;
	BOOST_FOREACH(path_flow_type pf, desc->simulation->path_flows) {
		std::set<path_handle>::iterator it = std::find_if(paths.begin(), paths.end(),
			boost::bind(&path::get_name, _1) == pf.second.second->name);

		if (paths.end() == it) {
			logger.error()() << "no path of given name " << pf.second.second->name;
			return false;
		}

		path_handle p = *it;
		p->set_flow(pf.first, pf.second.first);
		logger.debug()() << "created flow of " << pf.second.first << " beginning at " <<
			pf.first << "s for path " << p->get_name();
	}

	return true;
}

void simulation::prepare_identifiers()
{
	BOOST_FOREACH(actuator_handle ac, actuators) {
		identifiers.insert(ac->get_name());
	}
	BOOST_FOREACH(flow_sensor_handle fs, flow_sensors) {
		identifiers.insert(fs->get_name());
	}
	BOOST_FOREACH(queue_sensor_handle qs, queue_sensors) {
		identifiers.insert(qs->get_name());
	}
}

} // namespace simulation
} // namespace simulator

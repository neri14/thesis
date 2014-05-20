#include "creator.h"

#include <boost/foreach.hpp>

namespace simulator {
namespace simulation {

creator::creator(cell_handle cell_, int max_create_rate_, int max_vehicle_speed_) :
	logger("creator"),
	cell(cell_),
	max_create_rate(max_create_rate_),
	max_vehicle_speed(max_vehicle_speed_)
{}

creator::~creator()
{
	while(!vehicles.empty()) {
		vehicles.pop();
	}
}

vehicle_handle creator::create(int time_tick)
{
	BOOST_FOREACH(path_handle p, paths) {
		int interval = get_interval(p->get_flow(time_tick));
		int offset = p->get_offset();
		if (interval && time_tick && time_tick % interval == offset) {
			vehicles.push(vehicle_handle(new vehicle(p->get_name(), p->get_cells(), max_vehicle_speed, time_tick)));
		}
	}

	if (!vehicles.empty() && !cell->is_occupied()) {
		vehicle_handle tmp = vehicles.front();
		vehicles.pop();
		return tmp;
	} else {
		return vehicle_handle();
	}
}

void creator::add_path(path_handle p)
{
	paths.insert(p);
}

int creator::get_queue_size()
{
	return vehicles.size();
}

int creator::get_interval(int flow) const
{
	if (!flow) {
		return 0;
	}
	return static_cast<int>(ceil(3600.0/flow));
}

}
}
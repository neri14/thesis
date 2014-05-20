#include "vehicle.h"

#include <csv_writer.h>

namespace simulator {
namespace simulation {

int id = 0;

vehicle::vehicle(std::string path_name_, std::queue<path_cell> vehicle_path_, int max_speed_, int start_tick_) :
	vehicle_path(vehicle_path_),
	speed(max_speed_),
	max_speed(max_speed_),
	stop_count(0),
	start_tick(start_tick_),
	stop_tick(0),
	path_name(path_name_)
{}

vehicle::~vehicle()
{}

std::queue<path_cell>& vehicle::get_path()
{
	return vehicle_path;
}

void vehicle::set_speed(int spd)
{
	if (speed != 0 && spd == 0) {
		++stop_count;
	}
	speed = spd;
}

int vehicle::get_speed() const
{
	return speed;
}

void vehicle::set_stop_tick(int tick)
{
	stop_tick = tick;
	int i = ++id;
	common::get_csv_writer_opt().add_value(i, "name", path_name);
	common::get_csv_writer_opt().add_value(i, "stops", stop_count);
	common::get_csv_writer_opt().add_value(i, "time", stop_tick == 0 ? 0 : stop_tick-start_tick);
}

}
}
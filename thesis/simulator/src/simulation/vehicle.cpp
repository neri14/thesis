#include "vehicle.h"

namespace simulator {
namespace simulation {

vehicle::vehicle(std::queue<path_cell> vehicle_path_, int max_speed_) :
	vehicle_path(vehicle_path_),
	speed(max_speed_),
	max_speed(max_speed_)
{}

std::queue<path_cell>& vehicle::get_path()
{
	return vehicle_path;
}

void vehicle::set_speed(int spd)
{
	speed = spd;
}

int vehicle::get_speed() const
{
	return speed;
}

}
}
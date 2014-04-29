#ifndef VEHICLE_H
#define VEHICLE_H

#include <boost/shared_ptr.hpp>
#include "path_cell.h"

namespace simulator {
namespace simulation {

class vehicle
{
public:
	vehicle(std::queue<path_cell> vehicle_path_, int max_speed_);

	std::queue<path_cell>& get_path();

	void set_speed(int spd);
	int get_speed() const;

	std::string last_visited;
private:
	std::queue<path_cell> vehicle_path;
	int speed;
	int max_speed;
};
typedef boost::shared_ptr<vehicle> vehicle_handle;

}
}

#endif /* VEHICLE_H */
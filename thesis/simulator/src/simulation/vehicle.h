#ifndef VEHICLE_H
#define VEHICLE_H

#include <boost/shared_ptr.hpp>
#include "path_cell.h"

namespace simulator {
namespace simulation {

class vehicle
{
public:
	vehicle(std::string path_name_, std::queue<path_cell> vehicle_path_, int max_speed_, int start_tick_);
	virtual ~vehicle();

	std::queue<path_cell>& get_path();

	void set_speed(int spd);
	int get_speed() const;

	void set_stop_tick(int tick);
	int get_current_speed_time();

	std::string last_visited;
private:
	std::queue<path_cell> vehicle_path;
	int speed;
	int max_speed;
	int stop_count;
	int start_tick;
	int stop_tick;

	std::string path_name;

	int current_speed_time;
};
typedef boost::shared_ptr<vehicle> vehicle_handle;

}
}

#endif /* VEHICLE_H */
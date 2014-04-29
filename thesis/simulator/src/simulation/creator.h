#ifndef CREATOR_H
#define CREATOR_H

#include "cell.h"
#include "path.h"
#include "vehicle.h"

namespace simulator {
namespace simulation {

class creator
{
public:
	creator(cell_handle cell_, int max_create_rate_, int max_vehicle_speed_);
	virtual ~creator();

	vehicle_handle create(int time_tick);

	void add_path(path_handle p);

private:
	int get_interval(int flow) const;
	cell_handle cell;
	int max_create_rate;
	int max_vehicle_speed;

	std::set<path_handle> paths;
	std::queue<vehicle_handle> vehicles;
};
typedef boost::shared_ptr<creator> creator_handle;

} // namespace simulation
} // namespace simulator

#endif /* CREATOR_H */
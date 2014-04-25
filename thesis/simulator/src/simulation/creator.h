#ifndef CREATOR_H
#define CREATOR_H

#include "cell.h"

namespace simulator {
namespace simulation {

class creator
{
public:
	creator(cell_handle cell_, int max_create_rate_, int max_vehicle_speed_);

	//TODO method "create"
	//creates given vehicle (with path) and sets it on starting cell
	//if cell is occupied queues vehicle

private:
	cell_handle cell;
	int max_create_rate;
	int max_vehicle_speed;
};
typedef boost::shared_ptr<creator> creator_handle;

} // namespace simulation
} // namespace simulator

#endif /* CREATOR_H */
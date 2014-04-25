#include "creator.h"

namespace simulator {
namespace simulation {

creator::creator(cell_handle cell_, int max_create_rate_, int max_vehicle_speed_) :
	cell(cell_),
	max_create_rate(max_create_rate_),
	max_vehicle_speed(max_vehicle_speed_)
{}

}
}
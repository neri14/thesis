#include "destroyer.h"
#include <boost/bind.hpp>

namespace simulator {
namespace simulation {

destroyer::destroyer(cell_handle cell_, int max_destroy_rate_) :
	cell(cell_),
	max_destroy_rate(max_destroy_rate_),
	destroy_interval(ceil(3600/max_destroy_rate_))
{}

bool destroyer::should_destroy(int time_tick)
{
/*	static int last_destroy_tick = 0;

	if (cell->is_occupied() && time_tick - last_destroy_tick > destroy_interval) {
		last_destroy_tick = time_tick;
		return true;
	}
	return false;
*/
	return true; //ugly hack
}

}
}
#ifndef DESTROYER_H
#define DESTROYER_H

#include "cell.h"

namespace simulator {
namespace simulation {

class destroyer
{
public:
	destroyer(cell_handle cell_, int max_destroy_rate_);

	bool should_destroy(int time_tick);

private:
	cell_handle cell;
	int max_destroy_rate;
	int destroy_interval;
};
typedef boost::shared_ptr<destroyer> destroyer_handle;

} // namespace simulation
} // namespace simulator

#endif /* DESTROYER_H */
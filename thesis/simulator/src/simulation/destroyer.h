#ifndef DESTROYER_H
#define DESTROYER_H

#include "cell.h"

namespace simulator {
namespace simulation {

class destroyer
{
public:
	destroyer(cell_handle cell_, int max_destroy_rate_);

	//TODO on time tick - destroys vehicles according to max_destroy_rate

private:
	cell_handle cell;
	int max_destroy_rate;
};
typedef boost::shared_ptr<destroyer> destroyer_handle;

} // namespace simulation
} // namespace simulator

#endif /* DESTROYER_H */
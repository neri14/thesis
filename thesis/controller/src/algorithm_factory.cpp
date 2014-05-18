#include "algorithm_factory.h"

#include "simple_algorithm.h"
#include "dynamic_algorithm.h"

namespace controller {

algorithm_handle algorithm_factory::create(const std::string &name,
	set_actuator_cb_type cb, controlled_area_data area)
{
	if (name == "simple") {
		return algorithm_handle(new simple_algorithm(cb, area));
	} else if (name == "dynamic") {
		return algorithm_handle(new dynamic_algorithm(cb, area));
	}
	return algorithm_handle();
}

} // namespace controller
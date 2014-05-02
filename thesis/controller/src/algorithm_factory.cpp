#include "algorithm_factory.h"

#include "simple_algorithm.h"

namespace controller {

algorithm_handle algorithm_factory::create(const std::string &name, set_actuator_cb_type cb)
{
	if (name == "simple") {
		return algorithm_handle(new simple_algorithm(cb));
	}
	return algorithm_handle();
}

} // namespace controller
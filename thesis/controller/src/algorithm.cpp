#include "algorithm.h"

namespace controller {

algorithm::algorithm(const std::string& name, set_actuator_cb_type cb) :
	logger(name),
	set_actuator_cb(cb)
{}

algorithm::~algorithm()
{}

} // namespace controller
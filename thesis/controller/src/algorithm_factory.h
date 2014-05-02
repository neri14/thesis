#ifndef ALGORITHM_FACTORY_H
#define ALGORITHM_FACTORY_H

#include "algorithm.h"

namespace controller {

class algorithm_factory
{
public:
	static algorithm_handle create(const std::string& name, set_actuator_cb_type cb);

private:
	algorithm_factory() {}
};

} // namespace controller

#endif /* ALGORITHM_FACTORY_H */
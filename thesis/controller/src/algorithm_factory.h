#ifndef ALGORITHM_FACTORY_H
#define ALGORITHM_FACTORY_H

#include "algorithm.h"
#include "controlled_area.h"

namespace controller {

class algorithm_factory
{
public:
	static algorithm_handle create(const std::string& name,
		set_actuator_cb_type cb, controlled_area_data area);

private:
	algorithm_factory() {}
};

} // namespace controller

#endif /* ALGORITHM_FACTORY_H */
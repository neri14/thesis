#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <map>

#include <my_logger.h>

#include "controlled_area.h"
#include "controller_def.h"
#include "algorithm_object.h"

namespace controller {

typedef boost::function<void(const std::string&, EActuatorState)> set_actuator_cb_type;
class algorithm
{
public:
	algorithm(const std::string& name, set_actuator_cb_type cb, controlled_area_data area_);
	virtual ~algorithm();

	virtual void on_queue_sensor_update(const std::string& name, int time_tick, int queue, int max_queue) = 0;
	virtual void on_flow_sensor_update(const std::string& name, int timt_tick, int flow) = 0;
	virtual void on_time_tick(int time_tick) = 0;

protected:
	std::string as_short_string(EActuatorState state);

	common::my_logger logger;
	set_actuator_cb_type set_actuator_cb;
	controlled_area_data area;

	std::map<std::string, algorithm_object_handle> objects;
};
typedef boost::shared_ptr<algorithm> algorithm_handle;

} // namespace controller

#endif /* ALGORITHM_H */
#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>

#include <my_logger.h>

namespace controller {

enum EActuatorState {
	EActuatorState_Off,
	EActuatorState_Red,
	EActuatorState_RedYellow,
	EActuatorState_Green,
	EActuatorState_Yellow
};

typedef boost::function<void(const std::string&, EActuatorState)> set_actuator_cb_type;
class algorithm
{
public:
	algorithm(const std::string& name, set_actuator_cb_type cb);
	virtual ~algorithm();

	virtual void on_queue_sensor_update(const std::string& name, int time_tick, int queue, int max_queue) = 0;
	virtual void on_flow_sensor_update(const std::string& name, int timt_tick, int flow) = 0;
	virtual void on_time_tick(int time_tick) = 0;

protected:
	common::my_logger logger;
	set_actuator_cb_type set_actuator_cb;
};
typedef boost::shared_ptr<algorithm> algorithm_handle;

} // namespace controller

#endif /* ALGORITHM_H */
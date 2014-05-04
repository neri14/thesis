#include <my_logger.h>
#include <dispatcher/event_dispatcher.h>

#include "algorithm.h"
#include "controlled_area.h"

namespace controller {

class controller_handler
{
public:
	controller_handler(controlled_area_data area_);
	virtual ~controller_handler();

	virtual void start();
	virtual void stop();

private:
	void on_queue_event(common::dispatcher::event_handle ev);
	void on_flow_event(common::dispatcher::event_handle ev);
	void on_time_event(common::dispatcher::event_handle ev);

	void set_actuator_state(const std::string& name, EActuatorState state);

	common::my_logger logger;

	controlled_area_data area;
	algorithm_handle alg;

	common::dispatcher::connection_handle queue_listener;
	common::dispatcher::connection_handle flow_listener;
	common::dispatcher::connection_handle time_listener;
};

} // namespace controller
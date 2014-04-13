#ifndef ACTUATOR_H
#define ACTUATOR_H

#include <dispatcher/event_dispatcher.h>
#include <dispatcher/event/event_types.h>
#include <dispatcher/event/payload/set_actuator_state_payload.h>
#include "cell.h"

namespace simulator {
namespace simulation {

class actuator
{
public:
	actuator(std::string area_name_, common::dispatcher::EEventScope area_scope_,
		std::string actuator_name_, cell_handle controlled_cell_,
		int controlled_exit_);

private:
	void on_event(common::dispatcher::event_handle ev);

	EExitState map_state(common::dispatcher::EActuatorState state);

	std::string area_name;
	common::dispatcher::EEventScope area_scope;

	std::string actuator_name;

	cell_handle controlled_cell;
	int controlled_exit;

	common::dispatcher::connection_handle listener;
};
typedef boost::shared_ptr<actuator> actuator_handle;

} // namespace simulation
} // namespace simulator

#endif /* ACTUATOR_H */
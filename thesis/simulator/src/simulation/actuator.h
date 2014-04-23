#ifndef ACTUATOR_H
#define ACTUATOR_H

#include <boost/optional.hpp>

#include <dispatcher/event_dispatcher.h>
#include <dispatcher/event/event_types.h>
#include <dispatcher/event/payload/set_actuator_state_payload.h>

#include "cell.h"

namespace simulator {
namespace simulation {

class actuator
{
public:
	actuator(std::string area_name_, EEventScope area_scope_,
		std::string actuator_name_, cell_handle controlled_cell_,
		int controlled_exit_);
	
	virtual ~actuator();

private:
	void on_set_state(common::dispatcher::event_handle ev);
	void on_time_tick(common::dispatcher::event_handle ev);

	EExitState map_state(common::dispatcher::EActuatorState state);

	common::my_logger logger;

	std::string area_name;
	EEventScope area_scope;

	std::string actuator_name;

	cell_handle controlled_cell;
	int controlled_exit;

	boost::optional<EExitState> pending_state;

	common::dispatcher::connection_handle set_state_listener;
	common::dispatcher::connection_handle time_tick_listener;
};
typedef boost::shared_ptr<actuator> actuator_handle;

} // namespace simulation
} // namespace simulator

#endif /* ACTUATOR_H */
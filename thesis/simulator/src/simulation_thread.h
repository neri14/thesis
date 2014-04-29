#include <common_thread.h>
#include <dispatcher/event_dispatcher.h>

#ifndef SIMULATION_THREAD_H
#define SIMULATION_THREAD_H

namespace simulator {

class simulation_thread : public common::common_thread
{
public:
	simulation_thread(int duration_);
	virtual ~simulation_thread();

protected:
	void prepare();
	void run_impl();
	void stop_impl();

	void on_simulation_state_calculated(common::dispatcher::event_handle ev);

	bool keep_alive;
	int duration;
	int time_tick;

	int last_received_tick_calculated;

	common::dispatcher::connection_handle state_calculated_listener;
};

} // namespace simulator

#endif /* SIMULATION_THREAD_H */
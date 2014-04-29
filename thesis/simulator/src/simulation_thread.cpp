#include "simulation_thread.h"

#include <dispatcher/event/payload/simulation_state_calculated.h>
#include <dispatcher/event/payload/time_tick_payload.h>

namespace simulator {
namespace constant {
const int wait_interval(10);
}

simulation_thread::simulation_thread(int duration_) :
	common::common_thread("simulation_thread"),
	keep_alive(true),
	duration(duration_),
	time_tick(0),
	last_received_tick_calculated(-1)
{}

simulation_thread::~simulation_thread()
{
	common::dispatcher::get_dispatcher().unregister_listener(state_calculated_listener);
}

void simulation_thread::prepare()
{
	state_calculated_listener = common::dispatcher::get_dispatcher().register_listener(
		EEventType_SimulationStateCalculated, EEventScope_Local,
		boost::bind(&simulation_thread::on_simulation_state_calculated, this, _1));
}

void simulation_thread::run_impl()
{
	while(keep_alive && time_tick < duration) {
		logger.info()() << "time tick " << time_tick;
		common::dispatcher::payload_handle payload(
			new common::dispatcher::time_tick_payload(time_tick));
		common::dispatcher::get_dispatcher().dispatch(common::dispatcher::event_handle(
			new common::dispatcher::event(EEventType_TimeTick, EEventScope_General, payload)));

		//boost::this_thread::sleep(boost::posix_time::seconds(1));
		boost::this_thread::sleep(boost::posix_time::milliseconds(10));

		int delay = 0;
		while(time_tick != last_received_tick_calculated) {
			boost::this_thread::sleep(boost::posix_time::milliseconds(constant::wait_interval));
			++delay;

			if (delay > 6000) {
				logger.error()() << "simulator hanged for over a minute, killing thread";
				return;
			}

			if (!keep_alive) {
				return;
			}
		}
		if (delay) {
			logger.warning()() << "simulator overloaded, time tick delay " << (delay*10) << "ms";
		}

		++time_tick;
	}
}

void simulation_thread::stop_impl()
{
	keep_alive = false;
}

void simulation_thread::on_simulation_state_calculated(common::dispatcher::event_handle ev)
{
	BOOST_ASSERT(EEventType_SimulationStateCalculated == ev->get_type());
	BOOST_ASSERT(EEventScope_Local == ev->get_scope());

	int tick = ev->get_payload<common::dispatcher::simulation_state_calculated>()->time_tick;
	last_received_tick_calculated = tick;
}

}
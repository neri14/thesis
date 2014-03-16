#include "event_spammer_thread.h"

#include <dispatcher/event/payload/basic_payload.h>

namespace dispatcher_generic_client {

event_spammer_thread::event_spammer_thread() :
	common::common_thread("event_spammer_thread"),
	keep_alive(true)
{}

event_spammer_thread::~event_spammer_thread()
{}

void event_spammer_thread::prepare()
{
	//register for event
}

void event_spammer_thread::run_impl()
{
	unsigned int count = 0;
	while (keep_alive) {
		common::dispatcher::event_handle ev(new common::dispatcher::event(
			common::dispatcher::EEventType_BasicInt, common::dispatcher::EEventScope_General,
				boost::shared_ptr< common::dispatcher::basic_payload<int> >(
					new common::dispatcher::basic_payload<int>(++count))));

		common::dispatcher::get_dispatcher().dispatch(ev);
		boost::this_thread::sleep(boost::posix_time::seconds(1));
	}
}

void event_spammer_thread::stop_impl()
{
	keep_alive = false;
	//unregister from event
}

void event_spammer_thread::on_event(common::dispatcher::event_handle ev)
{
//	if (ev->get_type() == EEventType_BasicInt && ev->get_scope() == EEventScope_General) {
//		logger->log_debug
//	}
}

} // namespace dispatcher_generic_client

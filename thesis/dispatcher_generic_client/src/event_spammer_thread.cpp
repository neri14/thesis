#include "event_spammer_thread.h"

#include <dispatcher/event/payload/basic_payload.h>

namespace dispatcher_generic_client {

event_spammer_thread::event_spammer_thread() :
	common::common_thread("event_spammer_thread"),
	keep_alive(true)
{}

event_spammer_thread::~event_spammer_thread()
{
	if (listener) {
		common::dispatcher::get_dispatcher().unregister_listener(listener);
	}
}

void event_spammer_thread::prepare()
{
	listener = common::dispatcher::get_dispatcher().register_listener(
		common::dispatcher::EEventType_BasicInt, common::dispatcher::EEventScope_General,
		boost::bind(&event_spammer_thread::on_event, this, _1));
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
		boost::this_thread::sleep(boost::posix_time::seconds(5));
	}
}

void event_spammer_thread::stop_impl()
{
	keep_alive = false;
}

void event_spammer_thread::on_event(common::dispatcher::event_handle ev)
{
	if (ev->get_type() == common::dispatcher::EEventType_BasicInt &&
			ev->get_scope() == common::dispatcher::EEventScope_General) {
		logger.info()() << "received event value: "
			<< ev->get_payload< common::dispatcher::basic_payload<int> >()->value;
	}
}

} // namespace dispatcher_generic_client

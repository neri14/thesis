#include "event_dispatcher.h"

#include <boost/foreach.hpp>

namespace common {
namespace dispatcher {

event_dispatcher::event_dispatcher()
{}

event_dispatcher::~event_dispatcher()
{}

void event_dispatcher::register_observer(EEventType type, EEventScope scope, observer_handle observer)
{
	observers.push_back(observer_container(type, scope, observer));
}

void event_dispatcher::dispatch(const base_event& ev)
{
	BOOST_FOREACH(observer_container obs, observers) {
		if (ev.get_type() == obs.type &&
				(ev.get_scope() == obs.scope || EEventScope_Any == obs.scope)) {
			obs.observer(ev);
		}
	}
}

} // namespace dispatcher
} // namespace common

#include "event_dispatcher.h"

#include <boost/foreach.hpp>

namespace common {
namespace dispatcher {

event_dispatcher::event_dispatcher() :
	logger("event_dispatcher")
{}

event_dispatcher::~event_dispatcher()
{}

connection_handle event_dispatcher::register_listener(
	EEventType type, EEventScope scope, listener_handle listener)
{
	logger.debug()() << "Registering listener for type " << type << " and scope " << scope;
	connection_handle handle = connection_handle(new listener_connection(type, scope, listener));
	listeners.insert(handle);
	return handle;
}

void event_dispatcher::unregister_listener(connection_handle listener)
{
	logger.debug()() << "Unregistering listener registered for type "
		<< listener->type << " and scope " << listener->scope;
	listeners.erase(listener);
}

void event_dispatcher::dispatch(event_handle ev)
{
	logger.info()() << "Dispatching event " << ev->get_type() << ", scope: " << ev->get_scope();
	int count = 0;
	BOOST_FOREACH(connection_handle listener, listeners) {
		if (ev->get_type() == listener->type &&
				(ev->get_scope() == listener->scope || EEventScope_Any == listener->scope)) {
			listener->listener(ev);
			++count;
		}
	}
	logger.debug()() << "Event dispatched to " << count << " listeners";
}

} // namespace dispatcher
} // namespace common

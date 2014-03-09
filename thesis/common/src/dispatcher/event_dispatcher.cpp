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

	boost::mutex::scoped_lock lock(mtx_listeners);
	listeners.insert(handle);
	return handle;
}

void event_dispatcher::unregister_listener(connection_handle listener)
{
	logger.debug()() << "Unregistering listener registered for type "
		<< listener->type << " and scope " << listener->scope;

	boost::mutex::scoped_lock lock(mtx_listeners);
	listeners.erase(listener);
}

void event_dispatcher::dispatch(event_handle ev)
{
	logger.debug()() << "Dispatching event of type " << ev->get_type()
		<< " and scope " << ev->get_scope();

	boost::mutex::scoped_lock lock(mtx_events);
	event_queue.push(ev);
	logger.debug()() << event_queue.size() << " events waiting for distribution";
}

void event_dispatcher::distribute()
{
	int ev_count = 0;
	for (event_handle ev = get_event(); ev; ev = get_event()) {
		boost::mutex::scoped_lock lock(mtx_listeners);
		int list_count = 0;
		BOOST_FOREACH(connection_handle listener, listeners) {
			if ((ev->get_type() == listener->type || EEventType_Any == listener->type) &&
				(ev->get_scope() == listener->scope || EEventScope_Any == listener->scope)) {
				listener->listener(ev);
				++list_count;
			}
		}
		logger.debug()() << "Event distributed to " << list_count << " listeners";
		++ev_count;
	}
	logger.debug()() << ev_count << " events distributed";
}

event_handle event_dispatcher::get_event()
{
	boost::mutex::scoped_lock lock(mtx_events);
	event_handle ev;
	if (!event_queue.empty()) {
		ev = event_queue.front();
		event_queue.pop();
	}

	return ev;
}

event_dispatcher& get_dispatcher()
{
	static boost::shared_ptr<event_dispatcher> dispatcher_instance;
	if (!dispatcher_instance) {
		dispatcher_instance.reset(new event_dispatcher());
	}
	return *dispatcher_instance;
}

} // namespace dispatcher
} // namespace common

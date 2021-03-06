#include "event_dispatcher.h"

#include <boost/foreach.hpp>

namespace common {
namespace dispatcher {

event_dispatcher::event_dispatcher() :
	logger("event_dispatcher")
{}

event_dispatcher::event_dispatcher(std::string& prefix) :
	logger(prefix + "_event_dispatcher")
{}

event_dispatcher::~event_dispatcher()
{}

connection_handle event_dispatcher::register_listener(
	EEventType type, EEventScope scope, listener_handle listener)
{
	logger.debug()() << "Registering listener for type " << type << " and scope " << scope;
	connection_handle handle = connection_handle(new listener_connection(type, scope, listener));

	boost::mutex::scoped_lock lock(mtx_listeners);
	if (register_callback) {
		register_callback(type, scope);
	}
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
	boost::mutex::scoped_lock lock(mtx_events);
	event_queue.push(ev);
}

void event_dispatcher::distribute()
{
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
		logger.debug()() << "Event (type=" << ev->get_type() << " scope=" << ev->get_scope()
			<< ") distributed to " << list_count << " listeners";
	}
}

void event_dispatcher::set_register_callback(register_callback_type cb)
{
	register_callback = cb;
}

void event_dispatcher::reset_register_callback()
{
	register_callback.clear();
}

const std::set<connection_handle>& event_dispatcher::get_listeners()
{
	return listeners;
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
	static event_dispatcher instance;
	return instance;
}

} // namespace dispatcher
} // namespace common

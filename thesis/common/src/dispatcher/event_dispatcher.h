#ifndef EVENT_DISPATCHER_H
#define EVENT_DISPATCHER_H

#include <set>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

#include <logger.h>
#include "event/event_types.h"
#include "event/base_event.h"

namespace common {
namespace dispatcher {

struct listener_connection;

typedef boost::shared_ptr<base_event> event_handle;
typedef boost::function<void(event_handle)> listener_handle;
typedef boost::shared_ptr<listener_connection> connection_handle;

struct listener_connection
{
	listener_connection(EEventType type_, EEventScope scope_, listener_handle listener_) :
		type(type_),
		scope(scope_),
		listener(listener_)
	{}

	EEventType type;
	EEventScope scope;
	listener_handle listener;
};

class event_dispatcher
{
public:
	event_dispatcher();
	virtual ~event_dispatcher();

	virtual connection_handle register_listener(
		EEventType type, EEventScope scope, listener_handle listener);

	virtual void unregister_listener(connection_handle connector);

	virtual void dispatch(event_handle ev);

protected:
	common::logger logger;
	std::set<connection_handle> listeners;
};

} // namespace dispatcher
} // namespace common

#endif // EVENT_DISPATCHER_H

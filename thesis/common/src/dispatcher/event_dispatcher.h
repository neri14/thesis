#ifndef EVENT_DISPATCHER_H
#define EVENT_DISPATCHER_H

#include <vector>
#include <boost/function.hpp>

#include "event/event_types.h"
#include "event/base_event.h"

namespace common {
namespace dispatcher {

typedef boost::function1<void, const base_event&> observer_handle;

struct observer_container
{
	observer_container(EEventType type_, EEventScope scope_, observer_handle observer_) :
		type(type_),
		scope(scope_),
		observer(observer_)
	{}

	EEventType type;
	EEventScope scope;
	observer_handle observer;
};

class event_dispatcher
{
public:
	event_dispatcher();
	virtual ~event_dispatcher();

	virtual void register_observer(EEventType type, EEventScope scope, observer_handle observer);

	virtual void dispatch(const base_event& ev);

protected:
	std::vector<observer_container> observers;
};

} // namespace dispatcher
} // namespace common

#endif // EVENT_DISPATCHER_H

#ifndef BASE_EVENT_H
#define BASE_EVENT_H

#include "event_types.h"

namespace common {
namespace dispatcher {

class base_event
{
public:
	base_event(const EEventType& type_, const EEventScope& scope_) :
		type(type_),
		scope(scope_)
	{}

	virtual ~base_event()
	{}

	const EEventType& get_type() const
	{
		return type;
	}

	const EEventScope& get_scope() const
	{
		return scope;
	}

protected:
	EEventType type;
	EEventScope scope;
};

} // namespace dispatcher
} // namespace common

#endif // BASE_EVENT_H

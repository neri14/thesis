#ifndef VALUE_EVENT_H
#define VALUE_EVENT_H

#include "base_event.h"

namespace common {
namespace dispatcher {

template <class T>
class value_event : public base_event
{
public:
	value_event(const EEventType& type_, const EEventScope& scope_, const T& payload_) :
		base_event(type_, scope_),
		payload(payload_)
	{}

	~value_event()
	{}

	const T& get_payload() const
	{
		return payload;
	}

	const T* operator->() const
	{
		return &payload;
	}

protected:
	T payload;
};

} // namespace dispatcher
} // namespace common

#endif // VALUE_EVENT_H

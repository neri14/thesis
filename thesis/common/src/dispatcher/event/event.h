#ifndef EVENT_H
#define EVENT_H

#include "event_types.h"
#include "payload/base_payload.h"

namespace common {
namespace dispatcher {

class event;
typedef boost::shared_ptr<event> event_handle;

class event
{
public:
	event(const EEventType& type_, const EEventScope& scope_, payload_handle payload_) :
		type(type_),
		scope(scope_),
		payload(payload_)
	{}

	event(const EEventType& type_, const EEventScope& scope_) :
		type(type_),
		scope(scope_)
	{}

	virtual ~event()
	{}

	const EEventType& get_type() const
	{
		return type;
	}

	const EEventScope& get_scope() const
	{
		return scope;
	}

	template <typename T>
	boost::shared_ptr<T> get_payload() const
	{
		return boost::static_pointer_cast<T>(payload);
	}

	std::string encode() const
	{
		if (! payload) {
			return std::string();
		}
		return std::string(reinterpret_cast<char*>(payload.get()), payload->size());
	}

	template <typename T>
	void decode(const std::string& str)
	{
		if (str.empty()) {
			return;
		}
		payload = payload_handle(new T(*reinterpret_cast<const T*>(str.c_str())));
	}

protected:
	EEventType type;
	EEventScope scope;
	payload_handle payload;
};

} // namespace dispatcher
} // namespace common

#endif // EVENT_H

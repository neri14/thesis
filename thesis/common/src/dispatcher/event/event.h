#ifndef EVENT_H
#define EVENT_H

#include "event_types.h"
#include "payload/base_payload.h"

#include <boost/shared_ptr.hpp>

namespace common {
namespace dispatcher {

class event;
typedef boost::shared_ptr<event> event_handle;

class event
{
public:
	event(const EEventType& type_, const EEventScope& scope_, payload_handle payload_, int origin_ = 0) :
		type(type_),
		scope(scope_),
		payload(payload_),
		origin(origin_)
	{}

	event(const EEventType& type_, const EEventScope& scope_, int origin_ = 0) :
		type(type_),
		scope(scope_),
		origin(origin_)
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
		if (!payload || !payload->size()) {
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

	int get_origin() const
	{
		return origin;
	}

protected:
	EEventType type;
	EEventScope scope;
	payload_handle payload;

	int origin;
};

} // namespace dispatcher
} // namespace common

#endif // EVENT_H

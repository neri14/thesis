#ifndef BASIC_PAYLOAD_H
#define BASIC_PAYLOAD_H

#include "base_payload.h"

namespace common {
namespace dispatcher {

//used for passing basic type payloads
template<typename T>
struct basic_payload : public base_payload
{
	basic_payload(const T& value_) :
		value(value_)
	{}

	T value;

	virtual int size()
	{
		return sizeof(basic_payload<T>);
	}
};

} // namespace dispatcher
} // namespace common

#endif // BASIC_PAYLOAD_H

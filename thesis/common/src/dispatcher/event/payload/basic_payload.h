#ifndef BASIC_PAYLOAD_H
#define BASIC_PAYLOAD_H

namespace common {
namespace dispatcher {

//used for passing basic type payloads
template<typename T>
struct basic_payload
{
	basic_payload(const T& value_) :
		value(value_)
	{}

	T value;
};

} // namespace dispatcher
} // namespace common

#endif // BASIC_PAYLOAD_H

#ifndef BASE_PAYLOAD_H
#define BASE_PAYLOAD_H

#include <boost/shared_ptr.hpp>

namespace common {
namespace dispatcher {

struct base_payload;
typedef boost::shared_ptr<base_payload> payload_handle;

struct base_payload
{
	virtual int size() = 0;
};

} // namespace dispatcher
} // namespace common

#endif // BASE_PAYLOAD_H

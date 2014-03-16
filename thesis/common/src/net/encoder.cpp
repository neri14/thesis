#include "encoder.h"

#include <iomanip>

namespace common {
namespace net {

bool encode(common::dispatcher::proto_event_handle proto, std::string& output)
{
	std::string proto_enc;
	if (!proto->SerializeToString(&proto_enc)) {
		return false;
	}

	std::ostringstream ss;
	ss << std::setw(7) << std::setfill('0') << proto_enc.length();
	if (ss.str().length() > 7) {
		return false;
	}

	output = ss.str() + proto_enc;

	return true;
}

bool encode(common::dispatcher::proto_register_handle proto, std::string& output)
{
	std::string proto_enc;
	if (!proto->SerializeToString(&proto_enc)) {
		return false;
	}

	std::ostringstream ss;
	ss << std::setw(7) << std::setfill('0') << proto_enc.length();
	if (ss.str().length() > 7) {
		return false;
	}

	output = ss.str() + proto_enc;

	return true;
}

int decode_header(std::string str)
{
	return boost::lexical_cast<int>(str);
}

} // namespace net
} // namespace common

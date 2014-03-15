#include "encoder.h"

namespace common {
namespace net {

bool encode(common::dispatcher::proto_event_handle proto, std::string& output)
{
	std::string proto_enc;
	if (!proto->SerializeToString(&proto_enc)) {
		return false;
	}

	std::ostringstream ss;
	ss << proto_enc.length() << proto_enc;
	output = ss.str();

	return true;
}

bool encode(common::dispatcher::proto_register_handle proto, std::string& output)
{
	std::string proto_enc;
	if (!proto->SerializeToString(&proto_enc)) {
		return false;
	}

	std::ostringstream ss;
	ss << proto_enc.length() << proto_enc;
	output = ss.str();

	return true;
}

int decode_header(std::string str)
{
	return boost::lexical_cast<int>(str);
}

} // namespace net
} // namespace common

#ifndef ENCODER_H
#define ENCODER_H

#include <dispatcher/parse_message.h>

#include <boost/lexical_cast.hpp>

namespace common {
namespace net {

bool encode(common::dispatcher::proto_event_handle proto, std::string& output);
bool encode(common::dispatcher::proto_register_handle proto, std::string& output);
int decode_header(std::string str);

} // namespace net
} // namespace common

#endif /* ENCODER_H */

#ifndef PARSE_MESSAGE_H
#define PARSE_MESSAGE_H

#include <proto/messages.pb.h>

#include "event/event.h"

namespace common {
namespace dispatcher {

typedef boost::shared_ptr<common::proto::Event> proto_event_handle;
typedef boost::shared_ptr<common::proto::Register> proto_register_handle;

event_handle parse(proto_event_handle p_ev, int origin = 0);
proto_event_handle parse(event_handle ev);

std::pair<EEventType, EEventScope> parse(proto_register_handle p_reg);
proto_register_handle parse(std::pair<EEventType, EEventScope> reg);

} // namespace dispatcher
} // namespace common

#endif // PARSE_MESSAGE_H

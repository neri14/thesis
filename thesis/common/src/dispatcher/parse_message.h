#ifndef PARSE_MESSAGE_H
#define PARSE_MESSAGE_H

#include <proto/messages.pb.h>

#include "event/event.h"
#include "event/payload/basic_payload.h"

namespace common {
namespace dispatcher {

typedef boost::shared_ptr<common::proto::Event> proto_event_handle;

event_handle parse(proto_event_handle p_ev)
{
	EEventType type = static_cast<EEventType>(p_ev->type());
	EEventScope scope = static_cast<EEventScope>(p_ev->scope());

	event_handle ev(new event(type, scope));

	switch (type) {	
		default:
		case EEventType_Default:
			break;
		case EEventType_BasicInt:
			ev->decode< basic_payload<int> >(p_ev->payload());
			break;
	}

	return ev;
}
/*
proto_event_handle parse(event_handle ev)
{
	proto_event_handle p_ev(new common::proto::Event());

	p_ev.set_type(ev.get_type());
	p_ev.set_scope(ev.get_scope());
	p_ev.set_payload(ev.encode());
}*/

} // namespace dispatcher
} // namespace common

#endif // PARSE_MESSAGE_H

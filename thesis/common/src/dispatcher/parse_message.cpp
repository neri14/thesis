#include "parse_message.h"

namespace common {
namespace dispatcher {

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

proto_event_handle parse(event_handle ev)
{
	proto_event_handle p_ev(new common::proto::Event());

	p_ev->set_type(ev->get_type());
	p_ev->set_scope(ev->get_scope());
	p_ev->set_payload(ev->encode());

	return p_ev;
}

std::pair<EEventType, EEventScope> parse(proto_register_handle p_reg)
{
	return std::make_pair(
		static_cast<EEventType>(p_reg->type()), static_cast<EEventScope>(p_reg->scope()));
}

proto_register_handle parse(std::pair<EEventType, EEventScope> reg)
{
	proto_register_handle p_reg(new common::proto::Register());

	p_reg->set_type(reg.first);
	p_reg->set_scope(reg.second);

	return p_reg;
}

} // namespace dispatcher
} // namespace common

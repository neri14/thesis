#include <test.h>
#include <gtest/gtest.h>

#include <dispatcher/parse_message.h>
#include <dispatcher/event/payload/basic_payload.h>

namespace common {
namespace dispatcher {

namespace constant {

const EEventType event_type(EEventType_BasicInt);
const EEventScope event_scope(EEventScope_General);

const int event_value(0xBEEF);

} // namespace constant

class ut_parse_message : public ::testing::Test
{
};

TEST_F(ut_parse_message, event_can_be_encoded_to_string_and_decoded_back)
{
	boost::shared_ptr< basic_payload<int> > payload_before(new basic_payload<int>(constant::event_value));
	event_handle ev_before(new event(constant::event_type, constant::event_scope, payload_before));

	proto_event_handle p_ev_before = parse(ev_before);

	std::string serialized;
	p_ev_before->SerializeToString(&serialized);

	proto_event_handle p_ev_after(new common::proto::Event());
	p_ev_after->ParseFromString(serialized);

	event_handle ev_after = parse(p_ev_after);
	boost::shared_ptr< basic_payload<int> > payload_after = ev_after->get_payload< basic_payload<int> >();

	EXPECT_EQ(ev_before->get_type(), ev_after->get_type());
	EXPECT_EQ(ev_before->get_scope(), ev_after->get_scope());
	EXPECT_EQ(payload_before->value, payload_after->value);
}

} // namespace dispatcher
} // namespace common

#include <gtest/gtest.h>
#include <boost/bind.hpp>

#include <dispatcher/event_dispatcher.h>
#include <dispatcher/event/value_event.h>
#include <dispatcher/event/payload/basic_payload.h>

namespace common {
namespace dispatcher {

namespace constant {
	const EEventType event_type_default(EEventType_Default);
	const EEventType event_type_basicint(EEventType_BasicInt);

	const EEventScope event_scope_specific(EEventScope_General);
	const EEventScope event_scope_specific_2(EEventScope_Scope_1);
	const EEventScope event_scope_any(EEventScope_Any);

	const int event_value(0xBEEF);
} // namespace constant

class ut_event_dispatcher : public ::testing::Test
{
protected:
	event_dispatcher disp;
	int events_received;

	connection_handle listener;

public:
	ut_event_dispatcher() :
		events_received(0)
	{}

	void on_event(event_handle ev)
	{
		ASSERT_EQ(constant::event_type_default, ev->get_type());
		EXPECT_EQ(constant::event_scope_specific, ev->get_scope());

		const value_event< basic_payload<int> >& v_ev =
			static_cast< const value_event< basic_payload<int> >& >(*ev);

		EXPECT_EQ(constant::event_value, v_ev.get_payload().value);

		++events_received;
	}

protected:
	void register_listener(const EEventType& type, const EEventScope& scope)
	{
		listener = disp.register_listener(type, scope,
			boost::bind(&ut_event_dispatcher::on_event, this, _1));
	}

	void unregister_listener()
	{
		disp.unregister_listener(listener);
	}

	void dispatch_event()
	{
		boost::shared_ptr< value_event< basic_payload<int> > > ev(
			new value_event< basic_payload<int> >(constant::event_type_default,
				constant::event_scope_specific, basic_payload<int>(constant::event_value)));

		EXPECT_EQ(constant::event_type_default, ev->get_type());
		EXPECT_EQ(constant::event_scope_specific, ev->get_scope());
		EXPECT_EQ(constant::event_value, ev->get_payload().value);

		disp.dispatch(ev);
	}
};

TEST_F(ut_event_dispatcher, event_is_dispatched_to_listener_registered_to_specific_event_scope_specific)
{
	register_listener(constant::event_type_default, constant::event_scope_specific);

	dispatch_event();
	EXPECT_EQ(1, events_received);

	unregister_listener();

	dispatch_event();
	EXPECT_EQ(1, events_received);
}

TEST_F(ut_event_dispatcher, event_is_dispatched_to_listener_registered_to_any_event_scope_specific)
{
	register_listener(constant::event_type_default, constant::event_scope_any);

	dispatch_event();
	EXPECT_EQ(1, events_received);

	unregister_listener();

	dispatch_event();
	EXPECT_EQ(1, events_received);
}

TEST_F(ut_event_dispatcher, event_is_not_dispatched_to_listener_registered_to_different_type)
{
	register_listener(constant::event_type_basicint, constant::event_scope_specific);

	dispatch_event();
	EXPECT_EQ(0, events_received);

	unregister_listener();

	dispatch_event();
	EXPECT_EQ(0, events_received);
}

TEST_F(ut_event_dispatcher, event_is_not_dispatched_to_listener_registered_to_different_scope)
{
	register_listener(constant::event_type_default, constant::event_scope_specific_2);

	dispatch_event();
	EXPECT_EQ(0, events_received);

	unregister_listener();

	dispatch_event();
	EXPECT_EQ(0, events_received);
}

} // namespace dispatcher
} // namespace common

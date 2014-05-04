#include <test.h>
#include <gtest/gtest.h>
#include <boost/bind.hpp>

#include <dispatcher/event/payload/time_tick_payload.h>
#include <dispatcher/event/payload/set_actuator_state_payload.h>
#include <dispatcher/event/payload/actuator_finished_payload.h>

#include "../mocks/cell_mock.h"
#include "../../src/simulation/actuator.h"

namespace simulator {
namespace simulation {

namespace constant {

const std::string area_name("AREA_NAME");
const EEventScope area_scope(EEventScope_Area101);
const std::string actuator_name("ACTUATOR_NAME");
const int controlled_exit(0);

}

using ::testing::Return;

class ut_actuator : public ::testing::Test
{
protected:
	boost::shared_ptr<cell_mock> c_mock;
	actuator act;
	common::dispatcher::connection_handle listener;
	bool actuator_finished;

	ut_actuator() :
		c_mock(new cell_mock()),
		act(constant::area_name, constant::area_scope,
			constant::actuator_name, c_mock, constant::controlled_exit),
		actuator_finished(false)
	{
		listener = common::dispatcher::get_dispatcher().register_listener(
			EEventType_ActuatorFinished, EEventScope_Local, 
			boost::bind(&ut_actuator::on_actuator_finished, this, _1));
	}

	~ut_actuator()
	{
		common::dispatcher::get_dispatcher().unregister_listener(listener);
	}

	void notify_set_actuator_state(common::dispatcher::EActuatorState state)
	{
		common::dispatcher::payload_handle payload(
			new common::dispatcher::set_actuator_state_payload(constant::actuator_name, state));
		common::dispatcher::get_dispatcher().dispatch(common::dispatcher::event_handle(
			new common::dispatcher::event(EEventType_SetActuatorState, constant::area_scope, payload)));
	}

	void notify_time_tick()
	{
		static int tick = 0;
		common::dispatcher::payload_handle payload(
			new common::dispatcher::time_tick_payload(++tick));
		common::dispatcher::get_dispatcher().dispatch(common::dispatcher::event_handle(
			new common::dispatcher::event(EEventType_TimeTick, EEventScope_General, payload)));
	}

	void on_actuator_finished(common::dispatcher::event_handle ev)
	{
		ASSERT_EQ(EEventType_ActuatorFinished, ev->get_type());
		EXPECT_EQ(EEventScope_Local, ev->get_scope());

		EXPECT_EQ(constant::actuator_name,
			ev->get_payload<common::dispatcher::actuator_finished_payload>()->get_actuator_name());

		actuator_finished = true;
	}

	void distribute_events()
	{
		common::dispatcher::get_dispatcher().distribute();
	}
};

TEST_F(ut_actuator, actuator_sets_cell_exit_state_on_time_tick)
{
	notify_time_tick();
	distribute_events();
	EXPECT_TRUE(actuator_finished);

	actuator_finished = false;

	notify_set_actuator_state(common::dispatcher::EActuatorState_Green);
	distribute_events();
	EXPECT_FALSE(actuator_finished);

	EXPECT_CALL(*c_mock, set_exit_state(constant::controlled_exit, EExitState_Green))
		.WillOnce(Return(true));

	notify_time_tick();
	distribute_events();
	EXPECT_TRUE(actuator_finished);	
}

} // namespace simulation
} // namespace simulator
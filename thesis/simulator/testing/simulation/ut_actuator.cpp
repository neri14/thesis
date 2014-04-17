#include <gtest/gtest.h>

#include "../../src/simulation/actuator.h"

namespace simulator {
namespace simulation {

class ut_actuator : public ::testing::Test
{
};

TEST_F(ut_actuator, actuator_sets_cell_exit_state_on_time_tick)
{
	//TODO add cell mock after creating method for setting exit state in cell
	//TODO actuator_sets_cell_exit_state_on_time_tick after adding cell_mock 
}

} // namespace simulation
} // namespace simulator
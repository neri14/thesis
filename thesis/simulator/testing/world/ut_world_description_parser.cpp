#include <test.h>
#include <gtest/gtest.h>

#include "../../src/world/world_description_parser.h"
#include "../../src/world/world_description_validator.h"

#include <config/config.h>

#include <boost/scoped_ptr.hpp>

namespace simulator {
namespace world {

namespace constant {
	const std::string filename("./simulator/testing/data/world.xml");

	const int number_of_nodes(22);
	const int number_of_connection(25);
	const int number_of_actuators(5);
	const int number_of_flow_sensors(3);
	const int number_of_queue_sensors(4);
	const int number_of_areas(1);
	const int number_of_paths(6);
} // namespace constant

class ut_world_description_parser : public ::testing::Test
{
protected:
	boost::scoped_ptr<world_description_parser> parser;

	ut_world_description_parser()
	{
		parser.reset(new world_description_parser(constant::filename));
	}
};

TEST_F(ut_world_description_parser, DISABLED_parse_file)
{
	ASSERT_TRUE(parser->parse());
	world_description_handle desc = parser->get_world_description();

	EXPECT_EQ(constant::number_of_nodes, desc->nodes.size());
	EXPECT_EQ(constant::number_of_connection, desc->connections.size());
	EXPECT_EQ(constant::number_of_actuators, desc->actuators.size());
	EXPECT_EQ(constant::number_of_flow_sensors, desc->flow_sensors.size());
	EXPECT_EQ(constant::number_of_queue_sensors, desc->queue_sensors.size());
	EXPECT_EQ(constant::number_of_areas, desc->areas.size());
	EXPECT_EQ(constant::number_of_paths, desc->paths.size());

	world_description_validator validator(desc);
	ASSERT_TRUE(validator.validate());
}

} // namespace world
} // namespace simulator

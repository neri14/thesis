#include <gtest/gtest.h>

#include "../src/world/world_description_parser.h"

#include <config/config.h>

#include <boost/scoped_ptr.hpp>

namespace simulator {
namespace world {

namespace constant {
	const std::string filename("./simulator/testing/data/world.xml");

	const int number_of_nodes(22);
} // namespace constant

class ut_world_description_parser : public ::testing::Test
{
protected:
	boost::scoped_ptr<world_description_parser> parser;

	ut_world_description_parser()
	{
		common::get_config().add("log_error", "1");
		common::get_config().add("log_warning", "1");
		common::get_config().add("log_info", "1");
		common::get_config().add("log_debug", "1");
		common::get_config().add("log_std_out", "1");

		parser.reset(new world_description_parser(constant::filename));
	}
};

TEST_F(ut_world_description_parser, parse_file)
{
	ASSERT_TRUE(parser->parse());
	world_description_handle desc = parser->get_world_description();

	EXPECT_EQ(constant::number_of_nodes, desc->nodes.size());
}

} // namespace world
} // namespace simulator

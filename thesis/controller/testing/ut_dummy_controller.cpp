#include <gtest/gtest.h>

class ut_dummy_controller : public ::testing::Test
{
};

TEST_F(ut_dummy_controller, dummy)
{
	EXPECT_TRUE(true);
}

#include <gtest/gtest.h>

class ut_dummy_simulator : public ::testing::Test
{
};

TEST_F(ut_dummy_simulator, dummy)
{
	EXPECT_TRUE(true);
}

#include <gtest/gtest.h>

class ut_dummy_visualiser : public ::testing::Test
{
};

TEST_F(ut_dummy_visualiser, dummy)
{
	EXPECT_TRUE(true);
}

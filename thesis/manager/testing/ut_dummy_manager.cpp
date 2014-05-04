#include <test.h>
#include <gtest/gtest.h>

class ut_dummy_manager : public ::testing::Test
{
};

TEST_F(ut_dummy_manager, dummy)
{
	EXPECT_TRUE(true);
}

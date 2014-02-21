#include <gtest/gtest.h>

class ut_dummy_dispatcher_server : public ::testing::Test
{
};

TEST_F(ut_dummy_dispatcher_server, dummy)
{
	EXPECT_TRUE(true);
}

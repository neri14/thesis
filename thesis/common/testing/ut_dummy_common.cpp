#include <gtest/gtest.h>
#include <gmock/gmock.h>

class to_mock_cc
{
public:
	virtual void mhello() = 0;
};

class mock_cc : public to_mock_cc
{
public:
	MOCK_METHOD0(mhello, void());
};

class ut_dummy_common : public ::testing::Test
{
protected:
	mock_cc obj;
};

TEST_F(ut_dummy_common, dummy)
{
	EXPECT_CALL(obj, mhello()).Times(0);
	EXPECT_TRUE(true);
}

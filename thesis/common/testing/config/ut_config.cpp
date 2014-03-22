#include <gtest/gtest.h>

#include <config/config.h>

namespace common {

namespace constant {
	const std::string testfile("./common/testing/config/testfile.ini");

	const std::string key1("key1");
	const std::string key2("key2");
	const std::string key3("key3");

	const std::string value1("value 1");
	const std::string value2("value2");
	const int value3(1234);

	const int no_int_value(0);
} // namespace constant

class ut_config : public ::testing::Test
{
protected:
	config cfg;
};

TEST_F(ut_config, loading_testfile)
{
	ASSERT_TRUE(cfg.load_file(constant::testfile));

	EXPECT_EQ(constant::value1, cfg.get<std::string>(constant::key1));
	EXPECT_EQ(constant::value2, cfg.get<std::string>(constant::key2));
	EXPECT_EQ(constant::value3, cfg.get<int>(constant::key3));

	EXPECT_EQ(constant::no_int_value, cfg.get<int>(constant::key1));
}

} // namespace common

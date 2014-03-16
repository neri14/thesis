#ifndef MY_LOGGER_H
#define MY_LOGGER_H

#include <sstream>
#include <string>

#include <boost/shared_ptr.hpp>

#include "common_config.h"

namespace common {

class my_logger_stream
{
public:
	my_logger_stream(const ELogLevel& level, const std::string& prefix);
	virtual ~my_logger_stream();

	std::ostringstream& out();
	std::ostringstream& operator()();

private:
	std::string timestamp();
	std::string level_prefix(ELogLevel level) const;

	boost::shared_ptr<std::ostringstream> stream;
	bool level_allowed;
};

class my_logger
{
public:
	my_logger(std::string prefix_);

	my_logger_stream error();
	my_logger_stream warning();
	my_logger_stream info();
	my_logger_stream debug();

private:
	std::string prefix;
};

} // namespace common

#endif // MY_LOGGER_H

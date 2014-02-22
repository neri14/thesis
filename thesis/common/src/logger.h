#ifndef LOGGER_H
#define LOGGER_H

#include <sstream>
#include <string>

#include <boost/shared_ptr.hpp>

#include "common_config.h"

namespace common {

class logger_stream
{
public:
	logger_stream(const ELogLevel& level, const std::string& prefix);
	virtual ~logger_stream();

	std::ostringstream& out();
	std::ostringstream& operator()();

private:
	std::string timestamp();
	std::string level_prefix(ELogLevel level);

	boost::shared_ptr<std::ostringstream> stream;
	bool level_allowed;
};

class logger
{
public:
	logger(std::string prefix_);

	logger_stream error();
	logger_stream warning();
	logger_stream info();
	logger_stream debug();

private:
	std::string prefix;
};

} // namespace common

#endif // LOGGER_H

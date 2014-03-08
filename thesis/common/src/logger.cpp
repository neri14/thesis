#include "logger.h"

#include <iostream>
#include <sys/time.h>
#include <boost/lexical_cast.hpp>

namespace common {
namespace constant {
const int timestamp_buffer_size(20);
const std::string time_format("%Y%m%dT%H%M%S");
} // namespace constant

logger_stream::logger_stream(const ELogLevel& level, const std::string& prefix) :
	stream(new std::ostringstream()),
	level_allowed(level & common::config::log_level)
{
	if (level_allowed) {
		(*stream) << timestamp() << "\t" << level_prefix(level) << "\t" << prefix << ": ";
	}
}

logger_stream::~logger_stream()
{
	if (level_allowed) {
		if (common::config::log_output & ELogOutput_StdOut) {
			std::cout << stream->str() << std::endl;
		}
	}
}

std::ostringstream& logger_stream::out()
{
	return *stream;
}

std::ostringstream& logger_stream::operator()()
{
	return out();
}

std::string logger_stream::timestamp()
{
	timeval cur_time;
	gettimeofday(&cur_time, NULL);
	int milli = cur_time.tv_usec / 1000;

	char buffer[constant::timestamp_buffer_size];
	strftime(buffer, constant::timestamp_buffer_size,  constant::time_format.c_str(), localtime(&cur_time.tv_sec));

	std::ostringstream ss;
	ss << buffer << "." << milli;
	return ss.str();
}

std::string logger_stream::level_prefix(ELogLevel level)
{
	switch (level) {
		default:
		case ELogLevel_Info :
			return "[INF]";
		case ELogLevel_Error :
			return "[ERR]";
		case ELogLevel_Warning :
			return "[WRN]";
		case ELogLevel_Debug :
			return "[DBG]";
	}
}

logger::logger(std::string prefix_) :
	prefix(prefix_)
{}

logger_stream logger::error()
{
	return logger_stream(ELogLevel_Error, prefix);
}

logger_stream logger::warning()
{
	return logger_stream(ELogLevel_Warning, prefix);
}

logger_stream logger::info()
{
	return logger_stream(ELogLevel_Info, prefix);
}

logger_stream logger::debug()
{
	return logger_stream(ELogLevel_Debug, prefix);
}

} // namespace common

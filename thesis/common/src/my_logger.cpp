#include "my_logger.h"

#include <iostream>
#include <sys/time.h>
#include <boost/lexical_cast.hpp>

namespace common {
namespace constant {
const int timestamp_buffer_size(20);
const std::string time_format("%Y%m%dT%H%M%S");
} // namespace constant

my_logger_stream::my_logger_stream(const ELogLevel& level, const std::string& prefix) :
	stream(new std::ostringstream()),
	level_allowed(level & common::log_level)
{
	if (level_allowed) {
		(*stream) << timestamp() << "\t" << level_prefix(level) << "\t" << prefix << ": ";
	}
}

my_logger_stream::~my_logger_stream()
{
	if (level_allowed) {
		if (common::log_output & ELogOutput_StdOut) {
			std::cout << stream->str() << std::endl;
		}
	}
}

std::ostringstream& my_logger_stream::out()
{
	return *stream;
}

std::ostringstream& my_logger_stream::operator()()
{
	return out();
}

std::string my_logger_stream::timestamp()
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

std::string my_logger_stream::level_prefix(ELogLevel level) const
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

my_logger::my_logger(std::string prefix_) :
	prefix(prefix_)
{}

my_logger_stream my_logger::error()
{
	return my_logger_stream(ELogLevel_Error, prefix);
}

my_logger_stream my_logger::warning()
{
	return my_logger_stream(ELogLevel_Warning, prefix);
}

my_logger_stream my_logger::info()
{
	return my_logger_stream(ELogLevel_Info, prefix);
}

my_logger_stream my_logger::debug()
{
	return my_logger_stream(ELogLevel_Debug, prefix);
}

} // namespace common

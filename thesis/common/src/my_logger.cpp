#include "my_logger.h"

#include <iostream>
#include <sys/time.h>
#include <config/config.h>

#include <boost/lexical_cast.hpp>

namespace common {
namespace constant {
const int timestamp_buffer_size(20);
const std::string time_format("%Y%m%dT%H%M%S");
} // namespace constant

my_logger_stream::my_logger_stream(
		const ELogLevel& level, const std::string& prefix, int log_level, int log_output) :
	stream(new std::ostringstream()),
	level_allowed(level & log_level),
	allowed_output(log_output)
{
	if (level_allowed) {
		(*stream) << timestamp() << "\t" << level_prefix(level) << "\t" << prefix << ": ";
	}
}

my_logger_stream::~my_logger_stream()
{
	if (level_allowed) {
		if (allowed_output & ELogOutput_StdOut) {
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
	prefix(prefix_),
	log_level(0),
	log_output(0)
{
	common::config& cfg = common::get_config();
	log_level = log_level | (cfg.get<bool>("log_error") ? ELogLevel_Error : 0);
	log_level = log_level | (cfg.get<bool>("log_warning") ? ELogLevel_Warning : 0);
	log_level = log_level | (cfg.get<bool>("log_info") ? ELogLevel_Info : 0);
	log_level = log_level | (cfg.get<bool>("log_debug") ? ELogLevel_Debug : 0);
	log_output = log_output | (cfg.get<bool>("log_std_out") ? ELogOutput_StdOut : 0);
}

my_logger_stream my_logger::error()
{
	return my_logger_stream(ELogLevel_Error, prefix, log_level, log_output);
}

my_logger_stream my_logger::warning()
{
	return my_logger_stream(ELogLevel_Warning, prefix, log_level, log_output);
}

my_logger_stream my_logger::info()
{
	return my_logger_stream(ELogLevel_Info, prefix, log_level, log_output);
}

my_logger_stream my_logger::debug()
{
	return my_logger_stream(ELogLevel_Debug, prefix, log_level, log_output);
}

} // namespace common

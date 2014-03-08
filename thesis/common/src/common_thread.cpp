#include "common_thread.h"

#include <boost/bind.hpp>

namespace common {

common_thread::common_thread(const std::string& name) :
	logger(name)
{}

common_thread::~common_thread()
{}

void common_thread::start()
{
	logger.info()() << "starting thread";
	prepare();
	thread.reset(new boost::thread(boost::bind(&common_thread::run, this)));
	logger.info()() << "started thread";
}

void common_thread::stop()
{
	logger.info()() << "stopping thread";
	stop_impl();
	thread->join();
	logger.info()() << "stopped thread";
}

} // namespace common

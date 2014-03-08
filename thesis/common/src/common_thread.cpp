#include "common_thread.h"

#include <boost/bind.hpp>

namespace common {

common_thread::common_thread(const std::string& name) :
	logger(name),
	run_flag(false)
{}

common_thread::~common_thread()
{}

void common_thread::start()
{
	logger.info()() << "starting";
	run_flag = true;

	thread.reset(new boost::thread(boost::bind(&common_thread::run, this)));
}

void common_thread::stop()
{
	logger.info()() << "stopping";
	run_flag = false;
	thread->join();
	logger.info()() << "stopped";
}

void common_thread::run()
{
	logger.info()() << "preparing";
	prepare();

	logger.info()() << "entering loop";
	while (run_flag) {
		run_impl();
	}

	logger.info()() << "cleaning up";
	cleanup();
}

} // namespace common

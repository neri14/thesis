#include "common_thread.h"

#include <boost/bind.hpp>

namespace common {

common_thread::common_thread(const std::string& name) :
	logger(name),
	status(EThreadStatus_NotStarted)
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

void common_thread::run()
{
	status = EThreadStatus_Running;
	run_impl();
	status = EThreadStatus_Finished;
}

EThreadStatus common_thread::get_status()
{
	return status;
}

} // namespace common

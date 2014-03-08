#include "common_thread.h"

#include <boost/bind.hpp>

namespace common {

common_thread::common_thread(const std::string& name) :
	log(name),
	run_flag(false)
{}

common_thread::~common_thread()
{}

void common_thread::start()
{
	log.info()() << "starting";
	run_flag = true;

	thread.reset(new boost::thread(boost::bind(&common_thread::run, this)));
}

void common_thread::stop()
{
	log.info()() << "stopping";
	run_flag = false;
	thread->join();
	log.info()() << "stopped";
}

void common_thread::run()
{
	log.info()() << "preparing";
	prepare();

	log.info()() << "entering loop";
	while (run_flag) {
		run_impl();
	}

	log.info()() << "cleaning up";
	cleanup();
}

} // namespace common

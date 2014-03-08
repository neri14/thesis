#include "dispatcher_server_thread.h"

namespace dispatcher_server {

dispatcher_server_thread::dispatcher_server_thread() :
	common::common_thread("dispatcher_server_thread"),
	keep_alive(false)
{}

dispatcher_server_thread::~dispatcher_server_thread()
{}

void dispatcher_server_thread::prepare()
{
	keep_alive = true;
}

void dispatcher_server_thread::run()
{
	while (keep_alive) {
		boost::this_thread::sleep(boost::posix_time::seconds(1));
		logger.debug()() << "still alive";
	}
}

void dispatcher_server_thread::stop_impl()
{
	keep_alive = false;
}

} // namespace dispatcher_server

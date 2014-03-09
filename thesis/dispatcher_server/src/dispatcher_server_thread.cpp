#include "dispatcher_server_thread.h"
#include "tcp_server.h"

namespace dispatcher_server {

dispatcher_server_thread::dispatcher_server_thread(distributor_thread& distributor_) :
	common::common_thread("dispatcher_server_thread"),
	distributor(distributor_)
{}

dispatcher_server_thread::~dispatcher_server_thread()
{}

void dispatcher_server_thread::prepare()
{
}

void dispatcher_server_thread::run()
{
	logger.debug()() << "thread run begin";
	try {
		tcp_server server(io_service, distributor);
		io_service.run();
	} catch (std::exception& e) {
		logger.error()() << "network error: " << e.what();
	}
	logger.debug()() << "thread run end";
}

void dispatcher_server_thread::stop_impl()
{
	try {
		io_service.stop();
	} catch (std::exception& e) {
		logger.warning()() << "network error: " << e.what();
	}
}

} // namespace dispatcher_server

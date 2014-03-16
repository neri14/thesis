#include "dispatcher_client_thread.h"
#include "tcp_client.h"

namespace common {
namespace dispatcher_client {

dispatcher_client_thread::dispatcher_client_thread(common::dispatcher::distributor_thread& distributor_) :
	common::common_thread("dispatcher_client_thread"),
	distributor(distributor_)
{}

dispatcher_client_thread::~dispatcher_client_thread()
{}

void dispatcher_client_thread::prepare()
{}

void dispatcher_client_thread::run()
{
	logger.debug()() << "thread run begin";
	try {
		tcp_client client(io_service, distributor);
		io_service.run();
	} catch (std::exception& e) {
		logger.error()() << "network error: " << e.what();
	}
	logger.debug()() << "thread run end";
}

void dispatcher_client_thread::stop_impl()
{
	try {
		io_service.stop();
	} catch (std::exception& e) {
		logger.warning()() << "network error: " << e.what();
	}
}

} // namespace dispatcher_client
} // namespace common

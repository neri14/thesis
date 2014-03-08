#include "dispatcher_server_service.h"
#include "dispatcher_server_thread.h"

namespace dispatcher_server {

dispatcher_server_service::dispatcher_server_service() :
	logger("dispatcher_server_service")
{}

dispatcher_server_service::~dispatcher_server_service()
{}

int dispatcher_server_service::start()
{
	dispatcher_server_thread th;
	th.start();

	boost::this_thread::sleep(boost::posix_time::seconds(10));
	th.stop();

	return 0;
}

} // namespace dispatcher_server

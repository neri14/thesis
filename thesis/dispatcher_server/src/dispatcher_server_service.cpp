#include "dispatcher_server_service.h"

namespace dispatcher_server {

dispatcher_server_service::dispatcher_server_service() :
	logger("dispatcher_server_service")
{}

dispatcher_server_service::~dispatcher_server_service()
{}

int dispatcher_server_service::start()
{
	logger.debug()() << "not yet implemented";
	return 0;
}

} // namespace dispatcher_server

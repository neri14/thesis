#ifndef DISPATCHER_SERVER_SERVICE_H
#define DISPATCHER_SERVER_SERVICE_H

#include <common_service.h>
#include <my_logger.h>

namespace dispatcher_server {

class dispatcher_server_service : public common::common_service
{
public:
	dispatcher_server_service();
	virtual ~dispatcher_server_service();

	virtual int start();

private:
	common::my_logger logger;
};

} // namespace dispatcher_server

#endif /* DISPATCHER_SERVER_SERVICE_H */

#ifndef DISPATCHER_GENERIC_CLIENT_SERVICE_H
#define DISPATCHER_GENERIC_CLIENT_SERVICE_H

#include <common_service.h>
#include <my_logger.h>

namespace dispatcher_generic_client {

class dispatcher_generic_client_service : public common::common_service
{
public:
	dispatcher_generic_client_service();
	virtual ~dispatcher_generic_client_service();

	virtual int start();

private:
	common::my_logger logger;
};

} // namespace dispatcher_generic_client

#endif /* DISPATCHER_GENERIC_CLIENT_SERVICE_H */

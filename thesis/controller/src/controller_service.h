#ifndef CONTROLLER_SERVICE_H
#define CONTROLLER_SERVICE_H

#include <common_service.h>
#include <my_logger.h>

namespace controller {

class controller_service : public common::common_service
{
public:
	controller_service();
	virtual ~controller_service();

	virtual int start();

private:
	common::my_logger logger;
};

} // namespace controller

#endif /* CONTROLLER_SERVICE_H */
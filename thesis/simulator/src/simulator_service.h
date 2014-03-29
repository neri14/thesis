#ifndef SIMULATOR_SERVICE_H
#define SIMULATOR_SERVICE_H

#include <common_service.h>
#include <my_logger.h>

namespace simulator {

class simulator_service : public common::common_service
{
public:
	simulator_service();
	virtual ~simulator_service();

	virtual int start();

private:
	common::my_logger logger;
};

} // namespace simulator

#endif /* SIMULATOR_SERVICE_H */

#include <my_logger.h>
#include "algorithm.h"

namespace controller {

class controller_handler
{
public:
	controller_handler();
	virtual ~controller_handler();

	virtual void start();
	virtual void stop();

private:
	void set_actuator_state(const std::string& name, EActuatorState state);

	common::my_logger logger;

	algorithm_handle alg;
};

} // namespace controller
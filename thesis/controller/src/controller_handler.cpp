#include "controller_handler.h"

#include "algorithm_factory.h"
#include <config/config.h>

#include <boost/bind.hpp>

namespace controller {

controller_handler::controller_handler() :
	logger("controller_handler")
{
	alg = algorithm_factory::create(common::get_config().get<std::string>("algorithm"),
		boost::bind(&controller_handler::set_actuator_state, this, _1, _2));
}

controller_handler::~controller_handler()
{}

void controller_handler::start()
{
	logger.info()() << "starting control";
	//TODO load controlled description
	//TODO connect all needed events - (queue,flow,tick) should trigger functions in alg
}

void controller_handler::stop()
{
	//TODO disconnect events
}

void controller_handler::set_actuator_state(const std::string& name, EActuatorState state)
{
	//TODO send event to set state
}

} // namespace controller
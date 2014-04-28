#include "simulator_service.h"

#include "simulation/simulation.h"

#include <dispatcher/distributor_thread.h>
#include <dispatcher_client/dispatcher_client_thread.h>
#include <config/config.h>

namespace simulator {
namespace error_codes {
	const int exception(1);
	const int prepare_simulation(2);
}

simulator_service::simulator_service() :
	logger("simulator_service")
{}

simulator_service::~simulator_service()
{}

int simulator_service::start()
{
	try {
		common::dispatcher::distributor_thread distributor;
		distributor.start();

		common::dispatcher_client::dispatcher_client_thread client_thread(distributor);
		client_thread.start();

		simulation::simulation sim(common::get_config().get<std::string>("world_description_file"));
		if (!sim.prepare()) {
			return error_codes::prepare_simulation;
		}

		//TODO start simulation thread

		std::string str;
		while (str != "exit") {
			std::cout << "write \"exit\" to stop execution" << std::endl;
			std::cin >> str;
		}

		//TODO stop simulation thread
		client_thread.stop();
		distributor.stop();
	} catch (std::exception& e) {
		logger.error()() << "error: " << e.what();
		return error_codes::exception;
	}
	return 0;
}

} // namespace simulator

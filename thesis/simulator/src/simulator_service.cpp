#include "simulator_service.h"

#include <dispatcher/distributor_thread.h>
#include <dispatcher_client/dispatcher_client_thread.h>

namespace simulator {

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

		std::string str;
		while (str != "exit") {
			std::cout << "write \"exit\" to stop execution" << std::endl;
			std::cin >> str;
		}

		client_thread.stop();
		distributor.stop();
	} catch (std::exception& e) {
		logger.error()() << "error: " << e.what();
		return 1;
	}
	return 0;
}

} // namespace simulator

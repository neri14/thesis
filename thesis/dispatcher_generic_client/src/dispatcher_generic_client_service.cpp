#include "dispatcher_generic_client_service.h"

#include <dispatcher/distributor_thread.h>
#include <dispatcher_client/dispatcher_client_thread.h>

namespace dispatcher_generic_client {

dispatcher_generic_client_service::dispatcher_generic_client_service() :
	logger("dispatcher_generic_client_service")
{}

dispatcher_generic_client_service::~dispatcher_generic_client_service()
{}

int dispatcher_generic_client_service::start()
{
	try {
		common::dispatcher::distributor_thread distributor;
		distributor.start();

		common::dispatcher_client::dispatcher_client_thread client_thread(distributor);
		client_thread.start();

		std::string str;
		while (str != "stop") {
			std::cout << "write \"stop\" to stop execution" << std::endl;
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

} // namespace dispatcher_generic_client

#include "controller_service.h"

#include "controller_handler.h"
#include "controlled_area.h"

#include <dispatcher/distributor_thread.h>
#include <dispatcher_client/dispatcher_client_thread.h>
#include <config/config.h>

namespace controller {
namespace error_codes {
	const int exception(1);
	const int parsing_error(2);
}

controller_service::controller_service() :
	logger("controller_service")
{}

controller_service::~controller_service()
{}

int controller_service::start()
{
	try {
		common::dispatcher::distributor_thread distributor;
		distributor.start();

		common::dispatcher_client::dispatcher_client_thread client_thread(distributor);
		client_thread.start();

		controlled_area area(common::get_config().get<std::string>("controlled_area_file"));
		if (!area.parse()) {
			client_thread.stop();
			distributor.stop();
			return error_codes::parsing_error;
		}

		controller_handler ctrl;
		ctrl.start();

		std::string str;
		while (str != "exit") {
			std::cout << "write \"exit\" to stop execution" << std::endl;
			std::cin >> str;
		}

		ctrl.stop();
		client_thread.stop();
		distributor.stop();
	} catch(std::exception& e) {
		logger.error()() << "error: " << e.what();
		return error_codes::exception;
	}
	return 0;
}

} // namespace controller
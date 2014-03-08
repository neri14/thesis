#include "dispatcher_server_service.h"
#include "dispatcher_server_thread.h"

namespace dispatcher_server {

dispatcher_server_service::dispatcher_server_service() :
	logger("dispatcher_server_service")
{}

dispatcher_server_service::~dispatcher_server_service()
{}

int dispatcher_server_service::start()
{
	try {
		dispatcher_server_thread thread;
		thread.start();

		std::string str;
		while (str != "stop") {
			std::cout << "write \"stop\" to stop execution" << std::endl;
			std::cin >> str;
		}

		thread.stop();
	} catch (std::exception& e) {
		logger.error()() << "io_service error " << e.what();
	}

	return 0;
}

} // namespace dispatcher_server

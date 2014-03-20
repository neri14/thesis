#ifndef COMMON_MAIN_H
#define COMMON_MAIN_H

#include <my_logger.h>
#include <common_service.h>
#include <config/config.h>

#include <string>

namespace common {

template <class SERVICE>
int common_main(int argc, char ** argv, const char * name)
{
	if (!get_config().load(argc, argv)) {
		std::cerr << "config loading error" << std::endl;
		return 1;
	}

	my_logger logger(name);
	logger.info()() << "configfile: " << get_config().config_filename();
	logger.info()() << "=== STARTING UP ===";

	SERVICE service;
	int result = service.start();

	logger.info()() << "=== SHUTTING DOWN ===";
	return result;
}

}

#endif /* COMMON_MAIN_H */

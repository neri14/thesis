#ifndef COMMON_MAIN_H
#define COMMON_MAIN_H

#include <string>
#include <my_logger.h>
#include <common_service.h>

namespace common {

template <class SERVICE>
int common_main(int argc, char ** argv, const char * name)
{
	my_logger logger(name);
	logger.info()() << "=== STARTING UP ===";

	SERVICE service;
	int result = service.start();

	logger.info()() << "=== SHUTTING DOWN ===";
	return result;
}

}

#endif /* COMMON_MAIN_H */

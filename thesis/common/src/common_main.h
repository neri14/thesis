#ifndef COMMON_MAIN_H
#define COMMON_MAIN_H

#include <string>
#include <logger.h>
#include <common_service.h>

namespace common {

template <class SERVICE>
int common_main(int argc, char ** argv, const char * name)
{
	logger log(name);
	log.info()() << "=== STARTING UP ===";

	SERVICE service;
	int result = service.start();

	log.info()() << "=== SHUTTING DOWN ===";
	return result;
}

}

#endif /* COMMON_MAIN_H */

#include <common_main.h>
#include "../src/dispatcher_server_service.h"

int main(int argc, char** argv)
{
	return common::common_main<dispatcher_server::dispatcher_server_service>(
		argc, argv, "dispatcher_server_main");
}

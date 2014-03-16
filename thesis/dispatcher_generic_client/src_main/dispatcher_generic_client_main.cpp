#include <common_main.h>
#include "../src/dispatcher_generic_client_service.h"

int main(int argc, char** argv)
{
	return common::common_main<dispatcher_generic_client::dispatcher_generic_client_service>(
		argc, argv, "dispatcher_generic_client_main");
}

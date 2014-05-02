#include <common_main.h>
#include "../src/controller_service.h"

int main(int argc, char** argv)
{
	return common::common_main<controller::controller_service>(
		argc, argv, "controller_main");
}

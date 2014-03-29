#include <common_main.h>
#include "../src/simulator_service.h"

int main(int argc, char** argv)
{
	return common::common_main<simulator::simulator_service>(
		argc, argv, "simulator_main");
}

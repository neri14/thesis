#include "simulator_dispatcher.h"

namespace simulator {

common::dispatcher::event_dispatcher& get_dispatcher()
{
	static common::dispatcher::event_dispatcher instance;
	return instance;
}

}
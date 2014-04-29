#ifndef SIMULATION_STATE_CALCULATED_H
#define SIMULATION_STATE_CALCULATED_H

#include "base_payload.h"

namespace common {
namespace dispatcher {

struct simulation_state_calculated : public base_payload
{
	simulation_state_calculated(int time_tick_) :
		time_tick(time_tick_)
	{}

	virtual int size()
	{
		return sizeof(int);
	}

	int time_tick;
};

}
}

#endif /* SIMULATION_STATE_CALCULATED_H */
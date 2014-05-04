#ifndef TIME_TICK_PAYLOAD_H
#define TIME_TICK_PAYLOAD_H

#include "base_payload.h"

namespace common {
namespace dispatcher {

struct time_tick_payload : public base_payload
{
	time_tick_payload(int tick_) :
		tick(tick_)
	{}

	virtual int size()
	{
		return sizeof(time_tick_payload);
	}

	int tick;
};

}
}

#endif /* TIME_TICK_PAYLOAD_H */
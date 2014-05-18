#ifndef EXPECTED_FLOW_PAYLOAD_H
#define EXPECTED_FLOW_PAYLOAD_H

#include "base_payload.h"

namespace common {
namespace dispatcher {

struct expected_flow_payload : public base_payload
{
	expected_flow_payload(int origin_area_, int flow_, int time_tick_) :
		origin_area(origin_area_),
		flow(flow_),
		time_tick(time_tick_)
	{}

	virtual int size()
	{
		return sizeof(expected_flow_payload);
	}

	int origin_area;
	int flow;
	int time_tick;
};

}
}

#endif /* EXPECTED_FLOW_PAYLOAD_H */
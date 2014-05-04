#ifndef FLOW_SENSOR_STATE_H
#define FLOW_SENSOR_STATE_H

#include "base_payload.h"

#include <string.h>

namespace common {
namespace dispatcher {

struct flow_sensor_state : public base_payload
{
	flow_sensor_state(std::string sensor_name_, int flow_, int time_tick_) :
		length(std::min(EVENT_PAYLOAD_STRING_SIZE, static_cast<int>(sensor_name_.length()))),
		flow(flow_),
		time_tick(time_tick_)
	{
		memcpy(sensor_name, sensor_name_.c_str(), length);
	}

	virtual int size()
	{
//		int sensor_name_size = EVENT_PAYLOAD_STRING_SIZE;
//		return sensor_name_size*sizeof(char) + sizeof(int) * 3;
		return sizeof(flow_sensor_state);
	}

	std::string get_sensor_name()
	{
		return std::string(sensor_name, length);
	}

	int flow;
	int time_tick;
	int length;
	char sensor_name[EVENT_PAYLOAD_STRING_SIZE];
};

}
}

#endif /* FLOW_SENSOR_STATE_H */
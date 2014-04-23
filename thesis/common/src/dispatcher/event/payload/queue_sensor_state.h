#ifndef QUEUE_SENSOR_STATE_H
#define QUEUE_SENSOR_STATE_H

#include "base_payload.h"

#include <string.h>

namespace common {
namespace dispatcher {

struct queue_sensor_state : public base_payload
{
	queue_sensor_state(std::string sensor_name_, int queue_) :
		length(std::min(EVENT_PAYLOAD_STRING_SIZE, static_cast<int>(sensor_name_.length()))),
		queue(queue_)
	{
		memcpy(sensor_name, sensor_name_.c_str(), length);
	}

	virtual int size()
	{
		int sensor_name_size = EVENT_PAYLOAD_STRING_SIZE;
		return sensor_name_size*sizeof(char) + sizeof(int);
	}

	std::string get_actuator_name()
	{
		return std::string(sensor_name, length);
	}

	int length;
	char sensor_name[EVENT_PAYLOAD_STRING_SIZE];
	int queue;
};

}
}

#endif /* QUEUE_SENSOR_STATE_H */
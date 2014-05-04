#ifndef ACTUATOR_FINISHED_PAYLOAD_H
#define ACTUATOR_FINISHED_PAYLOAD_H

#include "base_payload.h"

#include <string>
#include <string.h>
#include <algorithm>

namespace common {
namespace dispatcher {

struct actuator_finished_payload : public base_payload
{
	actuator_finished_payload(std::string actuator_name_, int time_tick_) :
		length(std::min(EVENT_PAYLOAD_STRING_SIZE, static_cast<int>(actuator_name_.length()))),
		time_tick(time_tick_)
	{
		memcpy(actuator_name, actuator_name_.c_str(), length);
	}

	virtual int size()
	{
//		int actuator_name_size = EVENT_PAYLOAD_STRING_SIZE;
//		return actuator_name_size*sizeof(char) + sizeof(int) * 2;
		return sizeof(actuator_finished_payload);
	}

	std::string get_actuator_name()
	{
		return std::string(actuator_name, length);
	}

	int time_tick;
	int length;
	char actuator_name[EVENT_PAYLOAD_STRING_SIZE];
};

}
}

#endif /* ACTUATOR_FINISHED_PAYLOAD_H */
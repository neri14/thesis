#ifndef SET_ACTUATOR_STATE_PAYLOAD
#define SET_ACTUATOR_STATE_PAYLOAD

#include "base_payload.h"

#include <string.h>

namespace common {
namespace dispatcher {

enum EActuatorState {
	EActuatorState_Off,
	EActuatorState_Red,
	EActuatorState_RedYellow,
	EActuatorState_Green,
	EActuatorState_Yellow
};

struct set_actuator_state_payload : public base_payload
{
	set_actuator_state_payload(std::string actuator_name_, EActuatorState state_) :
		length(std::min(EVENT_PAYLOAD_STRING_SIZE, static_cast<int>(actuator_name_.length()))),
		state(state_)
	{
		memcpy(actuator_name, actuator_name_.c_str(), length);
	}

	virtual int size()
	{
		int actuator_name_size = EVENT_PAYLOAD_STRING_SIZE;
		return actuator_name_size*sizeof(char) + sizeof(EActuatorState) + sizeof(int);
	}

	std::string get_actuator_name()
	{
		return std::string(actuator_name, length);
	}

	int length;
	char actuator_name[EVENT_PAYLOAD_STRING_SIZE];
	EActuatorState state;
};

}
}

#endif /* SET_ACTUATOR_STATE_PAYLOAD */
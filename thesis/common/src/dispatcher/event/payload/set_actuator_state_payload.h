#ifndef SET_ACTUATOR_STATE_PAYLOAD
#define SET_ACTUATOR_STATE_PAYLOAD

#include "base_payload.h"

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
	set_actuator_state_payload(EActuatorState state_) :
		state(state_)
	{}

	virtual int size()
	{
		return sizeof(EActuatorState);
	}

	EActuatorState state;
};

}
}

#endif /* SET_ACTUATOR_STATE_PAYLOAD */
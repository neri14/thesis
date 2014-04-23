#ifndef EVENT_TYPES_H
#define EVENT_TYPES_H

enum EEventType
{
	EEventType_Any,      // observer can receive events of any type
	EEventType_Default,
	EEventType_BasicInt,
	EEventType_TimeTick,
	EEventType_SetActuatorState,
	EEventType_ActuatorFinished,
	EEventType_FlowSensorState,
	EEventType_QueueSensorState
};

enum EEventScope
{
	EEventScope_Any,     // observer can receive events from any scope
	EEventScope_General, // genaral event scope
	EEventScope_Local,   // event not to be passed over to event server
	EEventScope_Scope_1,
	EEventScope_Area101 = 101,
	EEventScope_Area102 = 102,
	EEventScope_Area103 = 103,
	EEventScope_Area104 = 104,
	EEventScope_Area105 = 105,
	EEventScope_Area106 = 106,
	EEventScope_Area107 = 107,
	EEventScope_Area108 = 108,
	EEventScope_Area109 = 109,
	EEventScope_Area110 = 110,
	EEventScope_Area111 = 111,
	EEventScope_Area112 = 112,
	EEventScope_Area113 = 113,
	EEventScope_Area114 = 114,
	EEventScope_Area115 = 115,
	EEventScope_Area116 = 116,
	EEventScope_Area117 = 117,
	EEventScope_Area118 = 118,
	EEventScope_Area119 = 119,
	EEventScope_Area120 = 120,
	EEventScope_Area121 = 121,
	EEventScope_Area122 = 122,
	EEventScope_Area123 = 123,
	EEventScope_Area124 = 124,
	EEventScope_Area125 = 125,
	EEventScope_Area126 = 126,
	EEventScope_Area127 = 127,
	EEventScope_Area128 = 128,
	EEventScope_Area129 = 129,
	EEventScope_Area130 = 130,
};

#endif // EVENT_TYPES_H

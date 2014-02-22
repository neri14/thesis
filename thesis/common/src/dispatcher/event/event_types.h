#ifndef EVENT_TYPES_H
#define EVENT_TYPES_H

namespace common {
namespace dispatcher {

enum EEventType
{
	EEventType_Default,
	EEventType_BasicInt
};

enum EEventScope
{
	EEventScope_Any,     // observer can receive events from any scope
	EEventScope_General,
	EEventScope_Scope_1
};

} // namespace dispatcher
} // namespace common

#endif // EVENT_TYPES_H

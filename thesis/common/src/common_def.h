#ifndef COMMON_DEF_H
#define COMMON_DEF_H

namespace common {

enum ELogLevel
{
	ELogLevel_Error = 1,
	ELogLevel_Warning = 2,
	ELogLevel_Info = 4,
	ELogLevel_Debug = 8
};

enum ELogOutput
{
	ELogOutput_StdOut = 1
};

} // namespace common

#endif // COMMON_DEF_H

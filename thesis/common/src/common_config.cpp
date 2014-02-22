#include "common_config.h"

namespace common {
namespace config {

int		log_level		= ELogLevel_Error | ELogLevel_Warning | ELogLevel_Info | ELogLevel_Debug;
int		log_output		= ELogOutput_StdOut;

} // namespace config
} // namespace common

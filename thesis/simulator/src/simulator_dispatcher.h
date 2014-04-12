#ifndef SIMULATOR_DISPATCHER_H
#define SIMULATOR_DISPATCHER_H

#include <dispatcher/event_dispatcher.h>

namespace simulator {

common::dispatcher::event_dispatcher& get_dispatcher();

} // namespace simulator

#endif /* SIMULATOR_DISPATCHER_H */
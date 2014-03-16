#ifndef EVENT_SPAMMER_THREAD_H
#define EVENT_SPAMMER_THREAD_H

#include <common_thread.h>
#include <dispatcher/event_dispatcher.h>

namespace dispatcher_generic_client {

class event_spammer_thread : public common::common_thread
{
public:
	event_spammer_thread();
	virtual ~event_spammer_thread();

protected:
	void prepare();
	void run_impl();
	void stop_impl();

	void on_event(common::dispatcher::event_handle ev);

	bool keep_alive;
	common::dispatcher::connection_handle listener;
};

} // namespace dispatcher_generic_client

#endif /* EVENT_SPAMMER_THREAD_H */

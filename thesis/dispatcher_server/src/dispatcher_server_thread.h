#ifndef DISPATCHER_SERVER_THREAD_H
#define DISPATCHER_SERVER_THREAD_H

#include <common_thread.h>

namespace dispatcher_server {

class dispatcher_server_thread : public common::common_thread
{
public:
	dispatcher_server_thread();
	virtual ~dispatcher_server_thread();

protected:
	void prepare();
	void run();
	void stop_impl();

	bool keep_alive;
};

} // namespace dispatcher_server

#endif /* DISPATCHER_SERVER_THREAD_H */

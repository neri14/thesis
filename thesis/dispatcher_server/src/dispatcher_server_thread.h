#ifndef DISPATCHER_SERVER_THREAD_H
#define DISPATCHER_SERVER_THREAD_H

#include <common_thread.h>

namespace dispatcher_server {

class dispatcher_server_thread : public common::common_thread
{
public:
	dispatcher_server_thread() :
		common::common_thread("dispatcher_server_thread")
	{}

	virtual ~dispatcher_server_thread()
	{}

protected:
	void prepare() {}
	void run_impl()
	{
		boost::this_thread::sleep(boost::posix_time::seconds(1));
		logger.debug()() << "still alive";
	}
	void cleanup() {}
};

} // namespace dispatcher_server

#endif /* DISPATCHER_SERVER_THREAD_H */

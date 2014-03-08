#ifndef COMMON_THREAD_H
#define COMMON_THREAD_H

#include <my_logger.h>
#include <boost/scoped_ptr.hpp>
#include <boost/thread.hpp>

namespace common {

class common_thread
{
public:
	common_thread(const std::string& name);
	virtual ~common_thread();

	//DO NOT implement implement these methods, implement prepare, run and stop_impl instead
	void start();
	void stop();

protected:
	virtual void prepare() = 0;
	virtual void run() = 0;
	virtual void stop_impl() = 0;

	boost::scoped_ptr<boost::thread> thread;
	my_logger logger;
};

}

#endif /* COMMON_THREAD_H */

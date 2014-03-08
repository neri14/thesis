#ifndef COMMON_THREAD_H
#define COMMON_THREAD_H

#include <logger.h>
#include <boost/scoped_ptr.hpp>
#include <boost/thread.hpp>

namespace common {

class common_thread
{
public:
	common_thread(const std::string& name);
	virtual ~common_thread();

	void start();
	void stop();

protected:
	void run();
	virtual void prepare() = 0;
	virtual void run_impl() = 0;
	virtual void cleanup() = 0;

	boost::scoped_ptr<boost::thread> thread;
	bool run_flag;
	logger log;
};

}

#endif /* COMMON_THREAD_H */

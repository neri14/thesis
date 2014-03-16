#ifndef COMMON_THREAD_H
#define COMMON_THREAD_H

#include <my_logger.h>
#include <boost/scoped_ptr.hpp>
#include <boost/thread.hpp>

namespace common {

enum EThreadStatus {
	EThreadStatus_NotStarted,
	EThreadStatus_Running,
	EThreadStatus_Finished
};

class common_thread
{
public:
	common_thread(const std::string& name);
	virtual ~common_thread();

	//DO NOT implement implement these methods, implement prepare, run_impl and stop_impl instead
	void start();
	void stop();

	EThreadStatus get_status();

private:
	void run();

protected:
	virtual void prepare() = 0;
	virtual void run_impl() = 0;
	virtual void stop_impl() = 0;

	boost::scoped_ptr<boost::thread> thread;
	my_logger logger;

	volatile EThreadStatus status;
};

}

#endif /* COMMON_THREAD_H */

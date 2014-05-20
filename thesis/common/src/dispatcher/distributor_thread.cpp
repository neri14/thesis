#include "distributor_thread.h"
#include <dispatcher/event_dispatcher.h>
#include <boost/foreach.hpp>

namespace common {
namespace dispatcher {

distributor_thread::distributor_thread() :
	common::common_thread("distributor_thread"),
	keep_alive(false),
	dispatched_count(0)
{}

distributor_thread::~distributor_thread()
{}

void distributor_thread::prepare()
{
	keep_alive = true;
}

void distributor_thread::run_impl()
{
	while (keep_alive) {
		boost::mutex::scoped_lock lock(dispatched_mtx);
		dispatched_count = 0;
		lock.unlock();
		get_dispatcher().distribute();

		BOOST_FOREACH(session_connection_handle session, sessions)
		{
			lock.lock();
			++dispatched_count;
			lock.unlock();

			session->cb();
		}

		int counter = 0;
		int seconds = 0;
		bool wait = true;
		while (wait) {
			if (counter++ >= 100) {
				++seconds;
				counter = 0;
				lock.lock();
				logger.error()() << "stuck for " << seconds << " seconds - waiting for " << dispatched_count << " sessions to finish";
				lock.unlock();
			}
			boost::this_thread::sleep(boost::posix_time::milliseconds(10));
			lock.lock();
			wait = (dispatched_count > 0);
			lock.unlock();
		}
	}
}

void distributor_thread::stop_impl()
{
	keep_alive = false;
}

void distributor_thread::session_finished()
{
	boost::mutex::scoped_lock lock(dispatched_mtx);
	--dispatched_count;
}

session_connection_handle distributor_thread::add_session(session_cb_type session)
{
	boost::mutex::scoped_lock lock(sessions_mtx);
	session_connection_handle conn(new session_connection(session));
	sessions.insert(conn);
	return conn;
}

void distributor_thread::remove_session(session_connection_handle session)
{
	boost::mutex::scoped_lock lock(sessions_mtx);
	sessions.erase(session);
}

} // namespace dispatcher
} // namespace common

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

void distributor_thread::run()
{
	while (keep_alive) {
		dispatched_count = 0;
		get_dispatcher().distribute();
		boost::mutex::scoped_lock lock(sessions_mtx);
		BOOST_FOREACH(session_connection_handle session, sessions)
		{
			session->cb();
			++dispatched_count;
		}
		lock.unlock();

		while (dispatched_count > 0) {
			boost::this_thread::sleep(boost::posix_time::milliseconds(10));
		}
		boost::this_thread::sleep(boost::posix_time::milliseconds(10));
	}
}

void distributor_thread::stop_impl()
{
	keep_alive = false;
}

void distributor_thread::session_finished()
{
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

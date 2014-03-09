#include "distributor_thread.h"
#include <dispatcher/event_dispatcher.h>
#include <boost/foreach.hpp>

namespace dispatcher_server {

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
		common::dispatcher::get_dispatcher().distribute();
		boost::mutex::scoped_lock(sessions_mtx);
		BOOST_FOREACH(session_connection_handle session, sessions)
		{
			session->cb();
			++dispatched_count;
		}

		while (dispatched_count > 0) {
			boost::this_thread::sleep(boost::posix_time::milliseconds(50));
		}
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
	boost::mutex::scoped_lock(sessions_mtx);
	session_connection_handle conn(new session_connection(session));
	sessions.push_back(conn);
}

void distributor_thread::remove_session(session_connection_handle session)
{
	boost::mutex::scoped_lock(sessions_mtx);

	for (std::vector<session_connection_handle>::iterator it = sessions.begin();
			it != sessions.end(); ++it) {
		if (*it == session) {
			sessions.erase(it);
			return;
		}
	}
}

} // namespace dispatcher_server

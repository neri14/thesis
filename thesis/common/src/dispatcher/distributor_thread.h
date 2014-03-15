#ifndef DISTRIBUTOR_THREAD_H
#define DISTRIBUTOR_THREAD_H

#include <vector>
#include <common_thread.h>

namespace common {
namespace dispatcher {

typedef boost::function<void()> session_cb_type;
struct session_connection
{
	session_connection(session_cb_type cb_) :
		cb(cb_)
	{}
	session_cb_type cb;
};
typedef boost::shared_ptr<session_connection> session_connection_handle;

class distributor_thread : public common::common_thread
{
public:
	distributor_thread();
	virtual ~distributor_thread();

	void session_finished();
	session_connection_handle add_session(session_cb_type session);
	void remove_session(session_connection_handle session);

protected:
	void prepare();
	void run();
	void stop_impl();

	bool keep_alive;
	int dispatched_count;

	boost::mutex sessions_mtx;
	std::vector<session_connection_handle> sessions;
};

} // namespace dispatcher
} // namespace common

#endif /* DISTRIBUTOR_THREAD_H */

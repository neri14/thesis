#ifndef TCP_SESSION_H
#define TCP_SESSION_H

#include "distributor_thread.h"

#include <dispatcher/event/event.h>
#include <dispatcher/event_dispatcher.h>
#include <my_logger.h>

#include <boost/thread/mutex.hpp>
#include <boost/function.hpp>
#include <boost/asio.hpp>

namespace dispatcher_server {

namespace constant {
	const int buffer_size(64*1024);
}

class tcp_session
{
public:
	typedef boost::function<void(tcp_session*)> session_exit_cb_type;
	typedef boost::function<void()> dispatch_cb_type;

	tcp_session(boost::asio::io_service& io_service_, session_exit_cb_type cb,
		distributor_thread& distributor_);
	virtual ~tcp_session();

	boost::asio::ip::tcp::socket& get_socket();
	void start();

private:
	void parse_message(std::string str);
	void add_event(common::dispatcher::event_handle e);
	void dispatch();
	void dispatch_impl();

	void handle_read(const boost::system::error_code& error, size_t bytes_transferred);
	void handle_write(const boost::system::error_code& error);
	std::string create_session_name(int id);

	int id;
	common::my_logger logger;
	boost::asio::ip::tcp::socket socket;
	distributor_thread& distributor;

	boost::mutex mtx_events;
	std::set<common::dispatcher::event_handle> events;
	std::vector<common::dispatcher::connection_handle> connections;

	session_exit_cb_type session_exit_cb;
	session_connection_handle distributor_connection;

	char in_buffer[constant::buffer_size];
	char out_buffer[constant::buffer_size];
};

} // namespace dispatcher_server

#endif /* TCP_SESSION_H */

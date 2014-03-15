#ifndef TCP_CLIENT_SESSION_H
#define TCP_CLIENT_SESSION_H

#include <dispatcher/distributor_thread.h>

#include <dispatcher/event/event.h>
#include <dispatcher/event_dispatcher.h>
#include <my_logger.h>

#include <boost/thread/mutex.hpp>
#include <boost/function.hpp>
#include <boost/asio.hpp>

namespace common {
namespace dispatcher_client {

namespace constant {
	const int buffer_size(1024*1024);
}

class tcp_client_session
{
public:
	typedef boost::function<void(tcp_client_session*)> session_exit_cb_type;
	typedef boost::function<void()> dispatch_cb_type;

	tcp_client_session(boost::asio::io_service& io_service_, session_exit_cb_type cb,
		common::dispatcher::distributor_thread& distributor_);
	virtual ~tcp_client_session();

	void connect();
	boost::asio::ip::tcp::socket& get_socket();
	void start();

private:
	void parse_message(std::string str);

	void add_event(common::dispatcher::event_handle e);
	void register_listener(common::dispatcher::EEventType type, common::dispatcher::EEventScope scope);

	void dispatch();
	void dispatch_impl();
	void dispatch_listeners();

	void read_header();
	void read_message(const boost::system::error_code& error, size_t bytes_transferred);
	void handle_read(const boost::system::error_code& error, size_t bytes_transferred);
	void handle_write(const boost::system::error_code& error);
	void handle_write_listener(const boost::system::error_code& error);
	std::string create_session_name(int id);

	int id;
	common::my_logger logger;
	boost::asio::ip::tcp::socket socket;
	common::dispatcher::distributor_thread& distributor;

	boost::mutex mtx_events;
	boost::mutex mtx_listeners;

	std::set<common::dispatcher::event_handle> events;
	std::set< std::pair<common::dispatcher::EEventType, common::dispatcher::EEventScope> > listeners;

	std::vector<common::dispatcher::connection_handle> connections;

	session_exit_cb_type session_exit_cb;
	common::dispatcher::session_connection_handle distributor_connection;

	char in_buffer[constant::buffer_size];
	char out_buffer[constant::buffer_size];
};

} // namespace dispatcher_client
} // namespace common

#endif /* TCP_CLIENT_SESSION_H */

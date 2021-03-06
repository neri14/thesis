#ifndef TCP_SERVER_SESSION_H
#define TCP_SERVER_SESSION_H

#include <dispatcher/distributor_thread.h>

#include <dispatcher/event/event.h>
#include <dispatcher/event_dispatcher.h>
#include <my_logger.h>

#include <boost/enable_shared_from_this.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/function.hpp>
#include <boost/asio.hpp>

namespace dispatcher_server {
namespace constant {
	const int buffer_size(1024*1024);
}

class tcp_server_session : public boost::enable_shared_from_this<tcp_server_session>
{
public:
	typedef boost::function<void(boost::shared_ptr<tcp_server_session>)> session_exit_cb_type;
	typedef boost::function<void()> dispatch_cb_type;

	tcp_server_session(boost::asio::io_service& io_service_, session_exit_cb_type cb,
		common::dispatcher::distributor_thread& distributor_);
	virtual ~tcp_server_session();

	void connect();
	boost::asio::ip::tcp::socket& get_socket();
	void start();

private:
	void parse_message(std::string str);
	void add_event(common::dispatcher::event_handle e);
	void dispatch();
	void dispatch_impl();

	void read_header();
	void read_message(const boost::system::error_code& error, size_t bytes_transferred);
	void handle_read(const boost::system::error_code& error, size_t bytes_transferred);
	void handle_write(const boost::system::error_code& error);
	std::string create_session_name(int id);

	int id;
	common::my_logger logger;
	boost::asio::ip::tcp::socket socket;
	common::dispatcher::distributor_thread& distributor;

	boost::mutex mtx_events;
	std::set<common::dispatcher::event_handle> events;
	std::vector<common::dispatcher::connection_handle> connections;

	session_exit_cb_type session_exit_cb;
	common::dispatcher::session_connection_handle distributor_connection;

	char in_buffer[constant::buffer_size];
	char out_buffer[constant::buffer_size];
};

} // namespace dispatcher_server

#endif /* TCP_SERVER_SESSION_H */

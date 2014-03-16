#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <set>

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>

#include <my_logger.h>
#include "tcp_server_session.h"

namespace dispatcher_server {

class tcp_server
{
public:
	tcp_server(boost::asio::io_service& io_service_, common::dispatcher::distributor_thread& distributor_);

private:
	void start_accept();
	void handle_accept(boost::shared_ptr<tcp_server_session> session,
		const boost::system::error_code& error);

	void session_exit(boost::shared_ptr<tcp_server_session> session_ptr);

	common::my_logger logger;
	boost::mutex mtx_sessions;

	boost::asio::io_service& io_service;
	boost::asio::ip::tcp::acceptor acceptor;
	common::dispatcher::distributor_thread& distributor;

	std::set< boost::shared_ptr<tcp_server_session> > active_sessions;
};

//tcp_server::last_id = 0;

} // namespace dispatcher_server

#endif /* TCP_SERVER_H */

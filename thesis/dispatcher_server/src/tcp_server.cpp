#include "tcp_server.h"
#include <boost/bind.hpp>

namespace dispatcher_server {
namespace constant {
	int port(56789);
}

tcp_server::tcp_server(boost::asio::io_service& io_service_,
		common::dispatcher::distributor_thread& distributor_) :
	logger("tcp_server"),
	io_service(io_service_),
	acceptor(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), constant::port)),
	distributor(distributor_)
{
	start_accept();
}

void tcp_server::start_accept()
{
	logger.debug()() << "preparing new session";
	boost::shared_ptr<tcp_server_session> session(
		new tcp_server_session(io_service, boost::bind(&tcp_server::session_exit, this, _1), distributor));
	session->connect();

	acceptor.async_accept(session->get_socket(),
		boost::bind(&tcp_server::handle_accept, this, session, boost::asio::placeholders::error));

	boost::mutex::scoped_lock lock(mtx_sessions);
	active_sessions.insert(session);
	logger.debug()() << "sessions count " << active_sessions.size();
}

void tcp_server::handle_accept(boost::shared_ptr<tcp_server_session> session,
	const boost::system::error_code& error)
{
	if (!error) {
		logger.info()() << "accepting new connection";
		session->start();
	} else {
		session_exit(session);
	}

	start_accept();
}

void tcp_server::session_exit(boost::shared_ptr<tcp_server_session> session_ptr)
{
	logger.debug()() << "session exit";
	boost::mutex::scoped_lock lock(mtx_sessions);
	active_sessions.erase(session_ptr);
	logger.debug()() << "sessions count " << active_sessions.size();
}

}

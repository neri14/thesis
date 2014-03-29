#include "tcp_client.h"

#include <config/config.h>

namespace common {
namespace dispatcher_client {

tcp_client::tcp_client(boost::asio::io_service& io_service_,
		common::dispatcher::distributor_thread& distributor_) :
	logger("tcp_client"),
	io_service(io_service_),
	distributor(distributor_)
{
	connect();
}

tcp_client::~tcp_client()
{
}

void tcp_client::connect()
{
	logger.debug()() << "preparing session";

	boost::mutex::scoped_lock lock(mtx_session);
	session.reset(
		new tcp_client_session(io_service,
			boost::bind(&tcp_client::session_exit, this), distributor));

	std::string host = get_config().get<std::string>("server_host");
	std::string port = get_config().get<std::string>("server_port");

	boost::asio::ip::tcp::resolver resolver(io_service);
	boost::asio::ip::tcp::resolver::query query(host, port);
	boost::asio::ip::tcp::resolver::iterator endpoint = resolver.resolve(query);

	logger.info()() << "connecting to server " << host << ":" << port;
	boost::asio::async_connect(session->get_socket(), endpoint,
		boost::bind(&tcp_client::handle_connect, this, boost::asio::placeholders::error));
}

void tcp_client::handle_connect(const boost::system::error_code& error)
{
	if (!error) {
		logger.info()() << "connected";
		boost::mutex::scoped_lock lock(mtx_session);

		session->connect();
		session->start();
	} else {
		logger.error()() << "could not connect to server";
		session_exit();
	}
}

void tcp_client::session_exit()
{
	logger.debug()() << "session exit";
	boost::mutex::scoped_lock lock(mtx_session);
	session.reset();
}

} // namespace dispatcher_client
} // namespace common

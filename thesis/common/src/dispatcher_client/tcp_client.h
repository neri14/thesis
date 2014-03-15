#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include "tcp_client_session.h"

#include <my_logger.h>
#include <dispatcher/distributor_thread.h>

#include <boost/asio.hpp>

namespace common {
namespace dispatcher_client {

class tcp_client
{
public:
	tcp_client(boost::asio::io_service& io_service_,
		common::dispatcher::distributor_thread& distributor_);
	virtual ~tcp_client();

private:
	void connect();
	void handle_connect(const boost::system::error_code& error);
	void session_exit();

	common::my_logger logger;
	boost::mutex mtx_session;
	boost::asio::io_service& io_service;
	common::dispatcher::distributor_thread& distributor;
	boost::shared_ptr<tcp_client_session> session;
};

} // namespace dispatcher_client
} // namespace common

#endif /* TCP_CLIENT_H */

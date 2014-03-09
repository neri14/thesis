#ifndef DISPATCHER_SERVER_THREAD_H
#define DISPATCHER_SERVER_THREAD_H

#include "distributor_thread.h"

#include <common_thread.h>

#include <boost/asio.hpp>

namespace dispatcher_server {

class dispatcher_server_thread : public common::common_thread
{
public:
	dispatcher_server_thread(distributor_thread& distributor_);
	virtual ~dispatcher_server_thread();

protected:
	void prepare();
	void run();
	void stop_impl();

	boost::asio::io_service io_service;
	distributor_thread& distributor;
};

} // namespace dispatcher_server

#endif /* DISPATCHER_SERVER_THREAD_H */

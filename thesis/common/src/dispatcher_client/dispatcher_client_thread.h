#ifndef DISPATCHER_CLIENT_THREAD_H
#define DISPATCHER_CLIENT_THREAD_H

#include <dispatcher/distributor_thread.h>
#include <common_thread.h>

#include <boost/asio.hpp>

namespace common {
namespace dispatcher_client {

class dispatcher_client_thread : public common::common_thread
{
public:
	dispatcher_client_thread(common::dispatcher::distributor_thread& distributor_);
	virtual ~dispatcher_client_thread();

protected:
	void prepare();
	void run_impl();
	void stop_impl();

	boost::asio::io_service io_service;
	common::dispatcher::distributor_thread& distributor;
};

} // namespace dispatcher_client
} // namespace common

#endif /* DISPATCHER_CLIENT_THREAD_H */

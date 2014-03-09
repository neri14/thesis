#include "tcp_session.h"

#include <dispatcher/parse_message.h>
#include <dispatcher/event_dispatcher.h>

#include <boost/foreach.hpp>
#include <boost/bind.hpp>
#include <sstream>

namespace dispatcher_server {

int last_id = 0;

tcp_session::tcp_session(boost::asio::io_service& io_service_, session_exit_cb_type cb,
		distributor_thread& distributor_) :
	id(++last_id),
	logger(create_session_name(id)),
	socket(io_service_),
	distributor(distributor_),
	session_exit_cb(cb)
{
	distributor_connection = distributor.add_session(boost::bind(&tcp_session::dispatch, this));
}

tcp_session::~tcp_session()
{
	BOOST_FOREACH(common::dispatcher::connection_handle l, connections) {
		common::dispatcher::get_dispatcher().unregister_listener(l);
	}
	distributor.remove_session(distributor_connection);
}

boost::asio::ip::tcp::socket& tcp_session::get_socket()
{
	return socket;
}

void tcp_session::start()
{
	logger.debug()() << "starting session";
	socket.async_read_some(boost::asio::buffer(in_buffer, constant::buffer_size),
		boost::bind(&tcp_session::handle_read, this,
			boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void tcp_session::handle_read(const boost::system::error_code& error,
	size_t bytes_transferred)
{
	if (!error) {
		parse_message(std::string(in_buffer, bytes_transferred));

		socket.async_read_some(boost::asio::buffer(in_buffer, constant::buffer_size),
			boost::bind(&tcp_session::handle_read, this,
				boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
	} else {
		logger.debug()() << "connection lost";
		session_exit_cb(this);
	}
}

void tcp_session::parse_message(std::string str)
{
	common::dispatcher::proto_event_handle event(new common::proto::Event());

	if (event->ParseFromString(str)) {
		logger.debug()() << "received event message";
		common::dispatcher::get_dispatcher().dispatch(common::dispatcher::parse(event));
		return;
	}

	common::proto::Register reg;
	if (reg.ParseFromString(str)) {
		logger.debug()() << "received register message";
		connections.push_back(
			common::dispatcher::get_dispatcher().register_listener(
				static_cast<common::dispatcher::EEventType>(reg.type()),
				static_cast<common::dispatcher::EEventScope>(reg.scope()),
				boost::bind(&tcp_session::add_event, this, _1)
			)
		);
	}
}

void tcp_session::add_event(common::dispatcher::event_handle e)
{
	boost::mutex::scoped_lock lock(mtx_events);
	events.insert(e);
	logger.debug()() << "added event to send";
}

void tcp_session::dispatch()
{
	logger.debug()() << "dispatching events triggered";
	dispatch_impl();
}

void tcp_session::dispatch_impl()
{
	boost::mutex::scoped_lock lock(mtx_events);
	if (events.size() <= 0) {
		distributor.session_finished();
		return;
	}

	logger.debug()() <<  events.size() << " events to send";
	common::dispatcher::event_handle ev = *(events.begin());
	events.erase(events.begin());
	lock.unlock();

	common::dispatcher::proto_event_handle proto = parse(ev);
	std::string str;
	if (proto->SerializeToString(&str)) {
		logger.debug()() << "sending event";
		memcpy(out_buffer, str.c_str(), str.length());
		boost::asio::async_write(socket, boost::asio::buffer(out_buffer, str.length()),
			boost::bind(&tcp_session::handle_write, this, boost::asio::placeholders::error));
	} else {
		logger.warning()() << "could not serializing event";
		dispatch_impl();
	}
}

void tcp_session::handle_write(const boost::system::error_code& error)
{
	if (!error) {
		logger.debug()() << "sent event";
		dispatch_impl();
	} else {
		logger.debug()() << "connection lost";
		session_exit_cb(this);
	}
}

std::string tcp_session::create_session_name(int id)
{
	std::ostringstream ss;
	ss << "tcp_session_" << id;
	return ss.str();
}

//distributed event dispatching:

// dispatching thread:
// 1. runs distribute on dispatcher
//   1a. each callback adds events to set - one copy only survives
// 2. runs dispatch_messages on each session and sets done callbacks (!!! callbacks will be propably run from different thread)
// 3. waits (some kind of wait/lock/thingy) for counter of callbacks to reach number of runned dispatch_messages
// 4. repeat

} // namespace dispatcher_server

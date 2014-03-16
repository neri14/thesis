#include "tcp_client_session.h"

#include <dispatcher/parse_message.h>
#include <dispatcher/event_dispatcher.h>
#include <net/encoder.h>

#include <boost/foreach.hpp>
#include <boost/bind.hpp>
#include <sstream>

namespace common {
namespace dispatcher_client {
namespace constant {
	const int header_size(7);
}

int last_id = 0;

tcp_client_session::tcp_client_session(boost::asio::io_service& io_service_, session_exit_cb_type cb,
		common::dispatcher::distributor_thread& distributor_) :
	id(++last_id),
	logger(create_session_name(id)),
	socket(io_service_),
	distributor(distributor_),
	session_exit_cb(cb)
{
}

void tcp_client_session::connect()
{
	distributor_connection = distributor.add_session(boost::bind(&tcp_client_session::dispatch, this));

	connections.push_back(
		common::dispatcher::get_dispatcher().register_listener(
			common::dispatcher::EEventType_Any, common::dispatcher::EEventScope_Any,
			boost::bind(&tcp_client_session::add_event, this, _1)
		)
	);

	common::dispatcher::get_dispatcher().set_register_callback(
		boost::bind(&tcp_client_session::register_listener, this, _1, _2));
}

tcp_client_session::~tcp_client_session()
{
	common::dispatcher::get_dispatcher().reset_register_callback();
	BOOST_FOREACH(common::dispatcher::connection_handle l, connections) {
		common::dispatcher::get_dispatcher().unregister_listener(l);
	}
	if (distributor_connection) {
		distributor.remove_session(distributor_connection);
	}
}

boost::asio::ip::tcp::socket& tcp_client_session::get_socket()
{
	return socket;
}

void tcp_client_session::start()
{
	logger.debug()() << "starting session";
	read_header();
}

void tcp_client_session::read_header()
{
	boost::asio::async_read(socket, boost::asio::buffer(in_buffer, constant::header_size),
		boost::bind(&tcp_client_session::read_message, this,
			boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void tcp_client_session::read_message(const boost::system::error_code& error,
	size_t bytes_transferred)
{
	if (!error) {
		int size = 0;
		try {
			size = common::net::decode_header(std::string(in_buffer, bytes_transferred));
		} catch(std::exception& e) {
			logger.error()() << "can't parse message header, killing connection";
			session_exit_cb();
		}
		boost::asio::async_read(socket, boost::asio::buffer(in_buffer, size),
			boost::bind(&tcp_client_session::handle_read, this,
				boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
	} else {
		logger.warning()() << "connection lost";
		session_exit_cb();
	}
}

void tcp_client_session::handle_read(const boost::system::error_code& error,
	size_t bytes_transferred)
{
	if (!error) {
		parse_message(std::string(in_buffer, bytes_transferred));
		read_header();
	} else {
		logger.debug()() << "connection lost";
		session_exit_cb();
	}
}

void tcp_client_session::parse_message(std::string str)
{
	common::dispatcher::proto_event_handle event(new common::proto::Event());

	if (event->ParseFromString(str)) {
		logger.debug()() << "received event message";
		common::dispatcher::get_dispatcher().dispatch(common::dispatcher::parse(event));
		return;
	}
}

void tcp_client_session::add_event(common::dispatcher::event_handle e)
{
	boost::mutex::scoped_lock lock(mtx_events);
	events.insert(e);
	logger.debug()() << "added event to send";
}

void tcp_client_session::register_listener(
	common::dispatcher::EEventType type, common::dispatcher::EEventScope scope)
{
	boost::mutex::scoped_lock lock(mtx_listeners);
	listeners.insert(std::make_pair(type, scope));
	logger.debug()() << "added listener to send";
}

void tcp_client_session::dispatch()
{
	dispatch_impl();
}

void tcp_client_session::dispatch_impl()
{
	boost::mutex::scoped_lock lock(mtx_events);
	if (events.size() <= 0) {
		lock.unlock();
		dispatch_listeners();
		return;
	}

	logger.debug()() <<  events.size() << " events to send";
	common::dispatcher::event_handle ev = *(events.begin());
	events.erase(events.begin());
	lock.unlock();

	common::dispatcher::proto_event_handle proto = parse(ev);
	std::string str;
	if (common::net::encode(proto, str)) {
		logger.debug()() << "sending event";
		memcpy(out_buffer, str.c_str(), str.length());
		boost::asio::async_write(socket, boost::asio::buffer(out_buffer, str.length()),
			boost::bind(&tcp_client_session::handle_write, this, boost::asio::placeholders::error));
	} else {
		logger.warning()() << "could not serialize event";
		dispatch_impl();
	}
}

void tcp_client_session::dispatch_listeners()
{
	boost::mutex::scoped_lock lock(mtx_listeners);

	if (listeners.size() <= 0) {
		distributor.session_finished();
		return;
	}
	logger.debug()() << listeners.size() << " listeners to send";
	std::pair<common::dispatcher::EEventType, common::dispatcher::EEventScope> listener =
		*(listeners.begin());
	listeners.erase(listeners.begin());
	lock.unlock();

	common::dispatcher::proto_register_handle proto = parse(listener);
	std::string str;
	if (common::net::encode(proto, str)) {
		logger.debug()() << "sending listener";
		memcpy(out_buffer, str.c_str(), str.length());
		boost::asio::async_write(socket, boost::asio::buffer(out_buffer, str.length()),
			boost::bind(&tcp_client_session::handle_write_listener, this, boost::asio::placeholders::error));
	} else {
		logger.warning()() << "could not serialize listener";
		dispatch_listeners();
	}
}

void tcp_client_session::handle_write(const boost::system::error_code& error)
{
	if (!error) {
		logger.debug()() << "sent event";
		dispatch_impl();
	} else {
		logger.debug()() << "connection lost";
		session_exit_cb();
	}
}

void tcp_client_session::handle_write_listener(const boost::system::error_code& error)
{
	if (!error) {
		logger.debug()() << "sent listener";
		dispatch_listeners();
	} else {
		logger.debug()() << "connection lost";
		session_exit_cb();
	}
}

std::string tcp_client_session::create_session_name(int id)
{
	std::ostringstream ss;
	ss << "tcp_client_session_" << id;
	return ss.str();
}

} // namespace dispatcher_client
} // namespace common

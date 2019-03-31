#pragma once

#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include "networkmessage.h"
#include "tools.h"

class Protocol;
using Protocol_ptr = std::shared_ptr<Protocol>;
class Connection;
using Connection_ptr = std::shared_ptr<Connection>;

class Connection : public std::enable_shared_from_this<Connection>
{
public:
	Connection(const Connection&) = delete;
	Connection& operator=(const Connection&) = delete;
	Connection(boost::asio::io_service& io_service)
		: io_service(io_service), socket(io_service)
	{
	}

	~Connection()
	{
		LOG_TRACE("Connection destroyed")
	}

	inline boost::asio::ip::tcp::socket& getSocket() { return socket; }

	std::string getIP();
	void establish();

private:
	boost::asio::io_service& io_service;
	boost::asio::ip::tcp::socket socket;
	Protocol_ptr protocol;
	NetworkMessage message;
	bool isEstablished = false;

	void parseHeader(const boost::system::error_code& error);
	void parsePacket(const boost::system::error_code& error);

	friend class Protocol;
};


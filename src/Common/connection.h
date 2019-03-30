#pragma once

#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include "networkmessage.h"

class Protocol;
using Protocol_ptr = std::shared_ptr<Protocol>;
class Connection;
using Connection_ptr = std::shared_ptr<Connection>;

class Connection : std::enable_shared_from_this<Connection>
{
public:
	Connection(boost::asio::io_service& io_service)
		: io_service(io_service), socket(io_service)
	{
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


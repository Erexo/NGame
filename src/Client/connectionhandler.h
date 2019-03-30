#pragma once

#include <boost/asio.hpp>
#include "connection.h"

enum ConnectionState
{
	Disconnected,
	Connected,
	// protocols
};

class ConnectionHandler
{
public:
	ConnectionHandler(std::string address, uint16_t port)
		: address(address), port(port), state(Disconnected)
	{
	}

	void connect();
	inline ConnectionState getState() { return state; }

private:
	boost::asio::io_service io_service;
	Connection_ptr activeConnection;
	ConnectionState state;
	std::string address;
	uint16_t port;
};


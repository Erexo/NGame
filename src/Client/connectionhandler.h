#pragma once

#include "connection.h"

class ConnectionHandler
{
public:
	ConnectionHandler(std::string address, uint16_t port)
		: address(address), port(port)
	{
	}

	void connect();

private:
	boost::asio::io_service io_service;
	Connection_ptr activeConnection;
	std::string address;
	uint16_t port;
};


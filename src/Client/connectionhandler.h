#pragma once

#include <Ntity/NtNet/connection.h>

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
	Ntity::NtNet::Connection_ptr activeConnection;
	std::string address;
	uint16_t port;
};


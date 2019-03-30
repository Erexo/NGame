#pragma once

#include <boost/asio.hpp>
#include <unordered_set>
#include "connection.h"
#include "service.h"

class ConnectionManager
{
public:
	static ConnectionManager& instance()
	{
		static ConnectionManager instance;
		return instance;
	}

	ConnectionManager(ConnectionManager&) = delete;
	ConnectionManager& operator=(ConnectionManager&) = delete;

	Connection_ptr createConnection(boost::asio::io_service& io_service, Service_ptr service);

private:
	ConnectionManager() = default;

	std::unordered_set<Connection_ptr> connections;
};


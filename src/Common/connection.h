#pragma once

#include <boost/system/config.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <unordered_set>
#include "networkmessage.h"
#include "tools.h"
#include "enums.h"

class Protocol;
using Protocol_ptr = std::shared_ptr<Protocol>;
class Connection;
using Connection_ptr = std::shared_ptr<Connection>;
class ConnectionManager;
using ConnectionManager_ptr = std::shared_ptr<ConnectionManager>;

class ConnectionManager
{
protected:
	virtual void releaseConnection(Connection_ptr connection) = 0;
	std::unordered_set<Connection_ptr> connections;

	virtual Connection_ptr createConnection() = 0;
	virtual void closeAll() = 0;

	friend class Connection;
};

class Connection : public std::enable_shared_from_this<Connection>
{
public:
	Connection(const Connection&) = delete;
	Connection& operator=(const Connection&) = delete;

	Connection(boost::asio::io_service& io_service, ConnectionManager_ptr connectionManager)
		: Connection(io_service)
	{
		this->connectionManager = connectionManager;
	}

	Connection(boost::asio::io_service& io_service)
		: io_service(io_service), socket(io_service), state(STATE_DISCONNECTED)
	{
		LOG_TRACE("Connection created")
	}

	~Connection()
	{
		closeSocket();
		LOG_TRACE("Connection destroyed")
	}

	inline boost::asio::ip::tcp::socket& getSocket() { return socket; }
	inline ConnectionState getState() { return state; }

	std::string getIP();
	void establish();
	void send(const NetworkMessage& msg);
	void close();

private:
	boost::asio::io_service& io_service;
	boost::asio::ip::tcp::socket socket;
	std::weak_ptr<ConnectionManager> connectionManager;
	Protocol_ptr protocol;
	NetworkMessage message;
	ConnectionState state;

	void closeSocket();

	void parseHeader(const boost::system::error_code& error);
	void parsePacket(const boost::system::error_code& error);

	void sendCallback(const boost::system::error_code& error);
	friend class Protocol;
};


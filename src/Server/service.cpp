#include "pch.h"
#include "service.h"

using namespace boost::asio::ip;

/// ConnectionManager
Connection_ptr Service::createConnection()
{
	auto connection = std::make_shared<Connection>(io_service, shared_from_this());
	makeProtocol(connection);
	connections.insert(connection);

	return connection;
}

void Service::releaseConnection(Connection_ptr connection)
{
	connections.erase(connection);
}

void Service::closeAll()
{
	for (auto& connection : connections)
		connection->close();

	connections.clear();
}
///

bool Service::open()
{
	close();

	try
	{
		acceptor.reset(new tcp::acceptor(io_service, tcp::endpoint(address::from_string("127.0.0.1"), port)));
		acceptor->set_option(tcp::no_delay(true));
		N_TRACE("Acceptor is set up, listening on port {0}", port);
		accept(); // begin accepting new connections
	}
	catch (boost::system::system_error& e)
	{
		N_ERROR(e.what());
		return false;
	}

	return true;
}

bool Service::close()
{
	if (acceptor && acceptor->is_open())
	{
		boost::system::error_code error;
		acceptor->close(error);
		if (!error)
			return true;

		N_ERROR(error.message());
	}

	return false;
}

Protocol_ptr Service::makeProtocol(Connection_ptr connection) const
{
	return factory->makeProtocol(connection);
}

void Service::accept()
{
	if (!acceptor)
	{
		N_WARN("acceptor does not exist");
		return;
	}

	auto connection = createConnection();

	N_DEBUG("Waiting for new connection");
	try
	{
		acceptor->async_accept(connection->getSocket(), std::bind(&Service::onAccept, shared_from_this(), connection, std::placeholders::_1));
	}
	catch (boost::system::system_error & e)
	{
		N_ERROR(e.what());
		connection->close();
	}
}

void Service::onAccept(Connection_ptr connection, const boost::system::error_code& error)
{
	if (error)
	{
		N_ERROR(error.message());
		connection->close();
		return;
	}

	auto remoteIP = connection->getIP();
	if (remoteIP.empty())
	{
		N_WARN("Corrupted connection");
		connection->close();
		return;
	}

	connection->establish();
	N_DEBUG("Connection established. Remote IP: {0}", remoteIP);

	accept(); // wait for new connection
}
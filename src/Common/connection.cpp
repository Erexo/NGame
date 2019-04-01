#include "connection.h"
#include "tools.h"

std::string Connection::getIP()
{
	boost::system::error_code error;
	auto endpoint = socket.remote_endpoint(error);
	if (error)
	{
		LOG_ERROR(error.message())
		return {};
	}

	return endpoint.address().to_string();
}

void Connection::establish()
{
	if (isEstablished)
	{
		LOG_TRACE("Connection already established")
		return;
	}

	isEstablished = true;

	try
	{
		boost::asio::async_read(socket, boost::asio::buffer(message.getBuffer(), NetworkMessage::HEADER_SIZE),
			std::bind(&Connection::parseHeader, shared_from_this(), std::placeholders::_1));
	}
	catch (boost::system::system_error& e)
	{
		LOG_ERROR(e.what())
		// todo: close
	}
}

void Connection::close()
{
	auto managerLock = connectionManager.lock();
	if (managerLock)
	{
		managerLock->releaseConnection(shared_from_this());
	}
}

void Connection::parseHeader(const boost::system::error_code& error)
{
	if (error)
	{
		LOG_ERROR(error.message())
		// todo: kill?
		return;
	}

	// todo: validate message

	LOG_TRACE("Header parsed, size=" << message.getLength())

	try
	{
		boost::asio::async_read(socket, boost::asio::buffer(message.getContentBuffer(), message.getLength()),
			std::bind(&Connection::parsePacket, shared_from_this(), std::placeholders::_1));
	}
	catch (boost::system::system_error& e)
	{
		LOG_ERROR(e.what())
		// todo: kill?
	}
}

void Connection::parsePacket(const boost::system::error_code& error)
{
	if (error)
	{
		LOG_ERROR(error.message())
		// todo: kill?
		return;
	}

	auto opcode = message.getByte();
	auto data = message.getString();
	LOG_TRACE("Packet parsed")

	try
	{
		boost::asio::async_read(socket, boost::asio::buffer(message.getBuffer(), NetworkMessage::HEADER_SIZE),
			std::bind(&Connection::parseHeader, shared_from_this(), std::placeholders::_1)); // read next message
	}
	catch (boost::system::system_error& e)
	{
		LOG_ERROR(e.what())
	}
}

void Connection::send(const NetworkMessage& msg)
{
	boost::asio::async_write(socket, boost::asio::buffer(msg.getBuffer(), msg.getLength() + NetworkMessage::HEADER_SIZE),
		std::bind(&Connection::sendCallback, shared_from_this(), std::placeholders::_1));
}

void Connection::sendCallback(const boost::system::error_code& error)
{
	if (error)
	{
		LOG_ERROR(error.message())
			// todo: kill?
			return;
	}

	LOG_TRACE("Message sent")
}
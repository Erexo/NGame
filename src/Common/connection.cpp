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
		boost::asio::async_read(socket, boost::asio::buffer(message.getLengthBuffer(), NetworkMessage::HEADER_SIZE),
			std::bind(&Connection::parseHeader, shared_from_this(), std::placeholders::_1));
	}
	catch (boost::system::system_error & e)
	{
		LOG_ERROR(e.what())
		// todo: close
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
		boost::asio::async_read(socket, boost::asio::buffer(message.getBuffer(), message.getLength()),
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

	LOG_TRACE("Packet parsed")

	try
	{
		boost::asio::async_read(socket, boost::asio::buffer(message.getLengthBuffer(), NetworkMessage::HEADER_SIZE),
			std::bind(&Connection::parseHeader, shared_from_this(), std::placeholders::_1)); // read next message
	}
	catch (boost::system::system_error& e)
	{
		LOG_ERROR(e.what())
	}
}

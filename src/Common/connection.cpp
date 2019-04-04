#include "connection.h"
#include "tools.h"
#include "protocol.h"

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
	if (state != STATE_DISCONNECTED)
	{
		LOG_TRACE("Connection already established")
		return;
	}

	state = STATE_CONNECTING;

	try
	{
		socket.async_receive(boost::asio::buffer(message.getBuffer(), NetworkMessage::HEADER_SIZE),
			std::bind(&Connection::parseHeader, shared_from_this(), std::placeholders::_1));
	}
	catch (boost::system::system_error& e)
	{
		LOG_ERROR(e.what())
		close();
	}
}

void Connection::close()
{
	auto managerLock = connectionManager.lock();
	if (managerLock)
	{
		managerLock->releaseConnection(shared_from_this());
	}

	closeSocket();
}

void Connection::closeSocket()
{
	state = STATE_CLOSED;

	if (socket.is_open())
	{
		try
		{
			socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
			socket.close();
		}
		catch (boost::system::system_error & e)
		{
			LOG_ERROR(e.what())
		}
	}
}

void Connection::parseHeader(const boost::system::error_code& error)
{
	if (error)
	{
		LOG_ERROR(error.message())
		close();
		return;
	}

	switch (state)
	{
	case STATE_CONNECTING:
		state = STATE_CONNECTED;
		break;
	case STATE_DISCONNECTED:
	case STATE_CLOSED:
		return;
	}

	// todo: validate message

	LOG_TRACE("Header parsed, size=" << message.getLength())

	try
	{
		socket.async_receive(boost::asio::buffer(message.getContentBuffer(), message.getLength()),
			std::bind(&Connection::parsePacket, shared_from_this(), std::placeholders::_1));
	}
	catch (boost::system::system_error& e)
	{
		LOG_ERROR(e.what())
		close();
	}
}

void Connection::parsePacket(const boost::system::error_code& error)
{
	message.reset();

	if (error)
	{
		LOG_ERROR(error.message())
		close();
		return;
	}

	if (protocol)
	{
		protocol->parsePacket(message);
	}
	else
	{
		LOG_TRACE("Protocol is not connected")
	}

	try
	{
		boost::asio::async_read(socket, boost::asio::buffer(message.getBuffer(), NetworkMessage::HEADER_SIZE),
			std::bind(&Connection::parseHeader, shared_from_this(), std::placeholders::_1)); // read next message
	}
	catch (boost::system::system_error& e)
	{
		LOG_ERROR(e.what())
		close();
	}
}

void Connection::send(const NetworkMessage& msg)
{
	try
	{
		boost::asio::async_write(socket, boost::asio::buffer(msg.getBuffer(), msg.getLength() + NetworkMessage::HEADER_SIZE),
		std::bind(&Connection::sendCallback, shared_from_this(), std::placeholders::_1));
	}
	catch (boost::system::system_error& e)
	{
		LOG_ERROR(e.what())
		close();
	}
}

void Connection::sendCallback(const boost::system::error_code& error)
{
	if (error)
	{
		LOG_ERROR(error.message())
		return;
	}

	LOG_TRACE("Message sent")
}
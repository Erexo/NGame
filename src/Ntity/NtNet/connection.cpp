#include <Ntity/pch.h>
#include "connection.h"
#include "protocol.h"

namespace Ntity
{
	namespace NtNet
	{
		std::string Connection::getIP()
		{
			boost::system::error_code error;
			auto endpoint = socket.remote_endpoint(error);
			if (error)
			{
				N_ERROR(error.message());
				return {};
			}

			return endpoint.address().to_string();
		}

		void Connection::establish()
		{
			if (state != ConnectionState::DISCONNECTED)
			{
				N_WARN("Connection already established");
				return;
			}

			state = ConnectionState::CONNECTING;

			try
			{
				readTimer.expires_from_now(boost::posix_time::seconds(CONNECTION_READ_TIMEOUT));
				readTimer.async_wait(std::bind(&Connection::handleTimeout, std::weak_ptr<Connection>(shared_from_this()), std::placeholders::_1));
				socket.async_receive(boost::asio::buffer(message.getBuffer(), NetworkMessage::HEADER_SIZE),
					std::bind(&Connection::parseHeader, shared_from_this(), std::placeholders::_1));
			}
			catch (boost::system::system_error & e)
			{
				N_ERROR(e.what());
				close();
			}
		}

		void Connection::close()
		{
			if (auto managerLock = connectionManager.lock())
				managerLock->releaseConnection(shared_from_this());

			closeSocket();
		}

		void Connection::closeSocket()
		{
			state = ConnectionState::CLOSED;

			if (socket.is_open())
			{
				readTimer.cancel();
				writeTimer.cancel();
				try
				{
					socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
					socket.close();
				}
				catch (boost::system::system_error & e)
				{
					N_ERROR(e.what());
				}
			}
		}

		void Connection::parseHeader(const boost::system::error_code & error)
		{
			readTimer.cancel();
			if (error)
			{
				N_ERROR(error.message());
				close();
				return;
			}

			switch (state)
			{
			case ConnectionState::CONNECTING:
				state = ConnectionState::CONNECTED;
				break;
			case ConnectionState::DISCONNECTED:
			case ConnectionState::CLOSED:
				return;
			}

			// todo: validate message

			N_DEBUG("Header parsed, size={0}", message.getLength());

			try
			{
				readTimer.expires_from_now(boost::posix_time::seconds(CONNECTION_READ_TIMEOUT));
				readTimer.async_wait(std::bind(&Connection::handleTimeout, std::weak_ptr<Connection>(shared_from_this()), std::placeholders::_1));
				socket.async_receive(boost::asio::buffer(message.getContentBuffer(), message.getLength()),
					std::bind(&Connection::parsePacket, shared_from_this(), std::placeholders::_1));
			}
			catch (boost::system::system_error & e)
			{
				N_ERROR(e.what());
				close();
			}
		}

		void Connection::parsePacket(const boost::system::error_code & error)
		{
			readTimer.cancel();
			message.reset();

			if (error)
			{
				N_ERROR(error.message());
				close();
				return;
			}

			if (protocol)
			{
				protocol->parsePacket(message);
			}
			else
			{
				N_WARN("Protocol is not connected");
			}

			try
			{
				readTimer.expires_from_now(boost::posix_time::seconds(CONNECTION_READ_TIMEOUT));
				readTimer.async_wait(std::bind(&Connection::handleTimeout, std::weak_ptr<Connection>(shared_from_this()), std::placeholders::_1));
				boost::asio::async_read(socket, boost::asio::buffer(message.getBuffer(), NetworkMessage::HEADER_SIZE),
					std::bind(&Connection::parseHeader, shared_from_this(), std::placeholders::_1)); // read next message
			}
			catch (boost::system::system_error & e)
			{
				N_ERROR(e.what());
				close();
			}
		}

		void Connection::send(const NetworkMessage & msg)
		{
			try
			{
				writeTimer.expires_from_now(boost::posix_time::seconds(CONNECTION_WRITE_TIMEOUT));
				writeTimer.async_wait(std::bind(&Connection::handleTimeout, std::weak_ptr<Connection>(shared_from_this()), std::placeholders::_1));
				boost::asio::async_write(socket, boost::asio::buffer(msg.getBuffer(), msg.getLength() + NetworkMessage::HEADER_SIZE),
					std::bind(&Connection::sendCallback, shared_from_this(), std::placeholders::_1));
			}
			catch (boost::system::system_error & e)
			{
				N_ERROR(e.what());
				close();
			}
		}

		void Connection::sendCallback(const boost::system::error_code & error)
		{
			writeTimer.cancel();
			if (error)
			{
				N_ERROR(error.message());
				return;
			}

			N_DEBUG("Message sent");
		}

		void Connection::handleTimeout(std::weak_ptr<Connection> connection, const boost::system::error_code & error)
		{
			if (error == boost::asio::error::operation_aborted)
				return;

			if (auto conn = connection.lock())
			{
				N_DEBUG("Timeout on connection with IP: {0}", conn->getIP());
			}

			if (auto conn = connection.lock())
				conn->close();
		}
	}
}
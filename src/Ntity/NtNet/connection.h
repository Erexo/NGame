#pragma once

#include "networkmessage.h"
#include "enums.h"

namespace Ntity
{
	namespace NtNet
	{
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
			static constexpr uint32_t CONNECTION_READ_TIMEOUT = 30;
			static constexpr uint32_t CONNECTION_WRITE_TIMEOUT = 30;

			Connection(const Connection&) = delete;
			Connection& operator=(const Connection&) = delete;

			Connection(boost::asio::io_service& io_service, ConnectionManager_ptr connectionManager)
				: Connection(io_service)
			{
				this->connectionManager = connectionManager;
			}

			Connection(boost::asio::io_service& io_service)
				: io_service(io_service), socket(io_service),
				readTimer(io_service), writeTimer(io_service),
				state(ConnectionState::DISCONNECTED)
			{
				N_DEBUG("Connection created");
			}

			~Connection()
			{
				closeSocket();
				N_DEBUG("Connection destroyed");
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
			boost::asio::deadline_timer readTimer;
			boost::asio::deadline_timer writeTimer;
			std::weak_ptr<ConnectionManager> connectionManager;
			Protocol_ptr protocol;
			NetworkMessage message;
			ConnectionState state;

			static void handleTimeout(std::weak_ptr<Connection> connection, const boost::system::error_code& error);

			void closeSocket();

			void parseHeader(const boost::system::error_code& error);
			void parsePacket(const boost::system::error_code& error);

			void sendCallback(const boost::system::error_code& error);

			friend class Protocol;
		};
	}
}
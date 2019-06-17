#pragma once

#include "protocol.h"

class Service;
using Service_ptr = std::shared_ptr<Service>;
class ProtocolFactoryBase;
using ProtocolFactory_ptr = std::shared_ptr<ProtocolFactoryBase>;

class Service : public ConnectionManager, public std::enable_shared_from_this<Service>
{
public:
	Service(boost::asio::io_service& io_service, uint16_t port, ProtocolFactory_ptr factory)
		: io_service(io_service), port(port), factory(factory)
	{
	}

	bool open();
	bool close();

	void onAccept(Connection_ptr connection, const boost::system::error_code& error);

private:
	boost::asio::io_service& io_service;
	uint16_t port;
	ProtocolFactory_ptr factory;
	std::unique_ptr<boost::asio::ip::tcp::acceptor> acceptor;

	// ConnectionManager
	virtual Connection_ptr createConnection() override;
	virtual void releaseConnection(Connection_ptr connection) override;
	virtual void closeAll() override;

	void accept();
	Protocol_ptr makeProtocol(Connection_ptr connection) const;
};


class ProtocolFactoryBase
{
public:
	virtual Protocol_ptr makeProtocol(Connection_ptr connection) const = 0;
};

template<typename ProtocolT>
class ProtocolFactory final : public ProtocolFactoryBase
{
public:
	Protocol_ptr makeProtocol(Connection_ptr connection) const override
	{
		auto protocol = std::make_shared<ProtocolT>();
		protocol->bindWithConnection(connection);
		N_DEBUG("Protocol created");
		return protocol;
	}
};
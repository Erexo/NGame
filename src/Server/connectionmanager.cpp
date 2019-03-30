#include <memory>
#include "connectionmanager.h"
#include "tools.h"

Connection_ptr ConnectionManager::createConnection(boost::asio::io_service& io_service, Service_ptr service)
{
	auto conn = std::make_shared<Connection>(io_service);
	auto protocol = service->makeProtocol(conn);


	LOG_TRACE("Connection created")
	connections.insert(conn);
	return conn;
}
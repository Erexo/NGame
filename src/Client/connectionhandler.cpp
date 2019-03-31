#include "connectionhandler.h"
#include "tools.h"
#include "protocol.h"

using namespace boost::asio::ip;

void ConnectionHandler::connect()
{
	if (state != Disconnected)
	{
		LOG_ERROR("Already connected")
		return;
	}

	activeConnection = std::make_shared<Connection>(io_service);
	auto protocol = std::make_shared<Protocol>();
	protocol->bindWithConnection(activeConnection);

	try
	{
		activeConnection->getSocket().connect(tcp::endpoint(address::from_string(address), port));
		LOG_TRACE("Connection established")
		activeConnection->establish();
	}
	catch (boost::system::system_error& e)
	{
		LOG_ERROR(e.what())
		return;
	}
}
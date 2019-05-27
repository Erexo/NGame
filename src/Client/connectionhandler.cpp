#include "pch.h"
#include "connectionhandler.h"
#include "tools.h"
#include "protocol.h"
#include "clientprotocol.h"
#include "enums.h"

using namespace boost::asio::ip;

void ConnectionHandler::connect()
{
	if (activeConnection && activeConnection->getState() == STATE_CONNECTED)
	{
		LOG_ERROR("Already connected")
		return;
	}

	activeConnection = std::make_shared<Connection>(io_service);
	Protocol_ptr protocol = std::make_shared<ClientProtocol>();
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
		activeConnection->close();
		return;
	}

	io_service.run();
}
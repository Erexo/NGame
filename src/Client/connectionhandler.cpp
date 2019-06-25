#include <Ntity/pch.h>
#include "connectionhandler.h"
#include "clientprotocol.h"

using namespace Ntity::NtNet;
using namespace boost::asio::ip;

void ConnectionHandler::connect()
{
	if (activeConnection && activeConnection->getState() == ConnectionState::CONNECTED)
	{
		N_WARN("Already connected");
		return;
	}

	activeConnection = std::make_shared<Connection>(io_service);
	Protocol_ptr protocol = std::make_shared<ClientProtocol>();
	protocol->bindWithConnection(activeConnection);

	try
	{
		activeConnection->getSocket().connect(tcp::endpoint(address::from_string(address), port));
		N_DEBUG("Connection established");
		activeConnection->establish();
	}
	catch (boost::system::system_error& e)
	{
		N_ERROR(e.what());
		activeConnection->close();
		return;
	}

	io_service.run();
}
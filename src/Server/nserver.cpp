#include <Ntity/pch.h>
#include "nserver.h"
#include "serverprotocol.h"

NServer::NServer()
{
	Ntity::Log::Init();
	N_TRACE("hi_server");
}

void NServer::Run()
{
	ServiceManager serviceManager;
	uint16_t port = 31463;
	serviceManager.add<ServerProtocol>(port);

	serviceManager.start();

	N_TRACE("server is running");

	while (serviceManager.poll());
}

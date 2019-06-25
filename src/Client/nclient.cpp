#include <Ntity/pch.h>
#include "nclient.h"

NClient::NClient()
	: connectionHandler("127.0.0.1", 31463)
{
	Ntity::Log::Init();
	N_TRACE("hi_client");
}

NClient::~NClient()
{

}

void NClient::Run()
{
	connectionHandler.connect();

	N_TRACE("client is running");

	while (true);


}

#include "pch.h"
#include "clientprotocol.h"
#include "enums.h"

void ClientProtocol::parsePacket(NetworkMessage& msg)
{
	auto opcode = (ServerOpcode)msg.getByte();
	N_DEBUG("Received opcode [{0}] from the server", opcode);

	switch (opcode)
	{
	case SERVER_BLANK:
		N_TRACE("Blank message was detected");
		break;
	case SERVER_DISCONNECT:
		connection->close();
		break;

	case SERVER_DISPLAY_MESSAGE:
		parseDisplayMessage(msg);
		break;

	default:
		N_WARN("Unknown message opcode: {0}", +opcode);
		break;
	}
}

void ClientProtocol::parseDisplayMessage(NetworkMessage& msg)
{
	std::string message = msg.getString();
	std::cout << "Message from server: " << message << std::endl;
}
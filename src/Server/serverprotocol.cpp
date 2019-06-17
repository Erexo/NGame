#include "pch.h"
#include "serverprotocol.h"
#include "enums.h"

void ServerProtocol::parsePacket(NetworkMessage& msg)
{
	auto opcode = (ClientOpcode)msg.getByte();
	N_DEBUG("Received opcode [{0}] from [{1}]", opcode, connection->getIP());

	switch (opcode)
	{
	case CLIENT_BLANK:
		N_TRACE("Blank message was detected");
		break;
	case CLIENT_DISCONNECT:
		connection->close();
		break;

	case CLIENT_DISPLAY_MESSAGE:
		parseDisplayMessage(msg);
		break;

	default:
		N_WARN("Unknown message opcode: {0}", opcode);
		break;
	}
}

void ServerProtocol::parseDisplayMessage(NetworkMessage& msg)
{
	std::string message = msg.getString();
	std::cout << "Message from client: " << message << std::endl;
}
#include "pch.h"
#include "serverprotocol.h"
#include "enums.h"
#include "tools.h"

void ServerProtocol::parsePacket(NetworkMessage& msg)
{
	auto opcode = (ClientOpcode)msg.getByte();
	LOG_TRACE("Received opcode [" << opcode << "] from [" << connection->getIP() << "]")

	switch (opcode)
	{
	case CLIENT_BLANK:
		LOG_TRACE("Blank message was detected")
		break;
	case CLIENT_DISCONNECT:
		connection->close();
		break;

	case CLIENT_DISPLAY_MESSAGE:
		parseDisplayMessage(msg);
		break;

	default:
		LOG_ERROR("Unknown message opcode: " << opcode)
		break;
	}
}

void ServerProtocol::parseDisplayMessage(NetworkMessage& msg)
{
	std::string message = msg.getString();
	std::cout << "Message from client: " << message << std::endl;
}
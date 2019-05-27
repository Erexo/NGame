#include "pch.h"
#include "clientprotocol.h"
#include "enums.h"
#include "tools.h"

void ClientProtocol::parsePacket(NetworkMessage& msg)
{
	auto opcode = (ServerOpcode)msg.getByte();
	LOG_TRACE("Received opcode [" << +opcode << "] from the server")

	switch (opcode)
	{
	case SERVER_BLANK:
		LOG_TRACE("Blank message was detected")
		break;
	case SERVER_DISCONNECT:
		connection->close();
		break;

	case SERVER_DISPLAY_MESSAGE:
		parseDisplayMessage(msg);
		break;

	default:
		LOG_ERROR("Unknown message opcode: " << +opcode)
		break;
	}
}

void ClientProtocol::parseDisplayMessage(NetworkMessage& msg)
{
	std::string message = msg.getString();
	std::cout << "Message from server: " << message << std::endl;
}
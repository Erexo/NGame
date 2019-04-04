#pragma once

#include "protocol.h"

class ServerProtocol : public Protocol
{
public:
	virtual void parsePacket(NetworkMessage& msg) override;

private:
	void parseDisplayMessage(NetworkMessage& msg);
};


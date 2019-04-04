#pragma once

#include "protocol.h"

class ClientProtocol : public Protocol
{
public:
	virtual void parsePacket(NetworkMessage& msg) override;

private:
	void parseDisplayMessage(NetworkMessage& msg);
};


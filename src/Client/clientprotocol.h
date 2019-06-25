#pragma once

#include <Ntity/NtNet/protocol.h>

class ClientProtocol : public Ntity::NtNet::Protocol
{
public:
	virtual void parsePacket(Ntity::NtNet::NetworkMessage& msg) override;

private:
	void parseDisplayMessage(Ntity::NtNet::NetworkMessage& msg);
};


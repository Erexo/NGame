#pragma once

#include <Ntity/Ntity.h>
#include <Ntity/NtNet/serverengine.h>
#include "servicemanager.h"

class NServer : public Ntity::NtNet::ServerEngine
{
public:
	NServer();
	virtual void Run() final;

private:
};

Ntity::Engine* Ntity::Create()
{
	return new NServer();
}
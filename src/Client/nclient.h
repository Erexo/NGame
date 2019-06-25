#pragma once

#include <Ntity/Ntity.h>
#include <Ntity/Graphics/renderingengine.h>
#include "connectionhandler.h"

class NClient : public Ntity::Graphics::RenderingEngine
{
public:
	NClient();
	~NClient();
	virtual void Run() final;

private:
	ConnectionHandler connectionHandler;
};

Ntity::Engine* Ntity::Create()
{
	return new NClient();
}
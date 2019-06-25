#pragma once

/*
	Ntity's entry point
*/

#include "engine.h"
#include "log.h"

extern Ntity::Engine* Ntity::Create();

int main(int argc, char* argv[])
{
	Ntity::Engine* engine = Ntity::Create();

	N_TRACE("hi_engine");
	
	if (!engine)
	{
		N_FATAL("No engine created");
		return 1;
	}
		
	engine->Run();
	delete engine;
	return 0;
}
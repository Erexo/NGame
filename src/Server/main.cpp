#include "pch.h"
#include "servicemanager.h"
#include "serverprotocol.h"

using namespace std;

int main(int argc, char* argv[])
{
	cout << "hi_server" << endl; 
	Log::Init();

	uint16_t port = argc > 1 ? atoi(argv[1]) : 31463;

	ServiceManager server;

	server.add<ServerProtocol>(port);

	server.start();

	cin.ignore();
	return 0;
}
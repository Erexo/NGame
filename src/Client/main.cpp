#include "pch.h"
#include "connectionhandler.h"

int main(int argc, char* argv[])
{
	std::cout << "hi_client" << std::endl;

	std::string address = argc > 1 ? argv[1] : "127.0.0.1";
	uint16_t port = argc > 2 ? atoi(argv[2]) : 31463;

	ConnectionHandler connectionHandler(address, port);
	connectionHandler.connect();

	std::cin.ignore();

	return 0;
}
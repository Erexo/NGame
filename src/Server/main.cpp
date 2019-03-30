#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "servicemanager.h"

using namespace std;

int main(void)
{
	cout << "hi_server" << endl;

	ServiceManager server;
	server.start();

	server.add<Protocol>(7171);


	cin.ignore();
	return 0;
}
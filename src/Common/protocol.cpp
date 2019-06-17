#include "pch.h"
#include "protocol.h"

void Protocol::bindWithConnection(Connection_ptr connection)
{
	if (this->connection || connection->protocol)
	{
		N_ERROR("Connection already binded");
		return;
	}

	this->connection = connection;
	connection->protocol = shared_from_this();
}
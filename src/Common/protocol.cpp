#include "pch.h"
#include "protocol.h"

void Protocol::bindWithConnection(Connection_ptr connection)
{
	if (this->connection || connection->protocol)
	{
		LOG_ERROR("Connection already binded")
		return;
	}

	this->connection = connection;
	connection->protocol = shared_from_this();
}
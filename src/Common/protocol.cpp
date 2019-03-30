#include "protocol.h"

void Protocol::bindWithConnection(Connection_ptr connection)
{
	if (connection)
	{
		LOG_ERROR("Connection already binded")
		return;
	}

	this->connection = connection;
}
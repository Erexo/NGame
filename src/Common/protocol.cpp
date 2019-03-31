#include "protocol.h"

void Protocol::bindWithConnection(Connection_ptr connection)
{
	if (this->connection)
	{
		LOG_ERROR("Connection already binded")
		return;
	}

	this->connection = connection;
}
#include <Ntity/pch.h>
#include "protocol.h"

namespace Ntity
{
	namespace NtNet
	{
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
	}
}
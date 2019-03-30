#pragma once

#include <boost/enable_shared_from_this.hpp>
#include "connection.h"
#include "tools.h"

class Protocol : public std::enable_shared_from_this<Protocol>
{
public:
	Protocol() = default;

	void bindWithConnection(Connection_ptr connection);
	
private:
	Connection_ptr connection;
};
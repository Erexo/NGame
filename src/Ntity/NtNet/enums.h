#pragma once

namespace Ntity
{
	namespace NtNet
	{
		enum class ConnectionState
		{
			DISCONNECTED,
			CONNECTING,
			CONNECTED,
			CLOSED
		};

		// todo: this should not be here
		enum ClientOpcode : uint8_t
		{
			CLIENT_BLANK = 0,
			CLIENT_DISCONNECT = 1,

			CLIENT_DISPLAY_MESSAGE = 2 // just for tests
		};

		// todo: this should not be here
		enum ServerOpcode : uint8_t
		{
			SERVER_BLANK = 0,
			SERVER_DISCONNECT = 1,

			SERVER_DISPLAY_MESSAGE = 2 // just for tests
		};
	}
}
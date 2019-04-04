#pragma once

enum ConnectionState
{
	STATE_DISCONNECTED,
	STATE_CONNECTING,
	STATE_CONNECTED,
	STATE_CLOSED
};

enum ClientOpcode : uint8_t
{
	CLIENT_BLANK = 0,
	CLIENT_DISCONNECT = 1,
	
	CLIENT_DISPLAY_MESSAGE = 2 // just for tests
};

enum ServerOpcode : uint8_t
{
	SERVER_BLANK = 0,
	SERVER_DISCONNECT = 1,

	SERVER_DISPLAY_MESSAGE = 2 // just for tests
};
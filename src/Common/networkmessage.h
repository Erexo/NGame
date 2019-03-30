#pragma once

#include <cstdint>
#include <cassert>


class NetworkMessage
{
using message_size = uint16_t;

public:
	static constexpr message_size MAX_SIZE = 24590;
	static constexpr int8_t HEADER_SIZE = sizeof(message_size);

	inline message_size getPosition() { return position; }
	inline message_size getLength() 
	{ 
		assert(length <= MAX_SIZE);
		return length; 
	}

	inline uint8_t* getLengthBuffer() { return (uint8_t*)&length; }
	inline uint8_t* getBuffer() { return buffer; }

private:
	uint8_t buffer[MAX_SIZE];
	message_size length = 0;
	message_size position = 0;

	inline bool canAdd(size_t size)
	{
		return position + size < MAX_SIZE;
	}

	inline bool canRead(size_t size)
	{
		return size <= position;
	}
};


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
	inline message_size getLength()  { return length; }

	inline uint8_t* getLengthBuffer() { return (uint8_t*)&length; }
	inline uint8_t* getBuffer() { return buffer; }

	uint8_t getByte();
	std::string getString();
	template <typename T>
	T get();

	bool addByte(uint8_t value);
	template<typename T>
	bool add(T value);
	bool addString(const std::string& value);

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
		return position + size < length;
	}
};

template <typename T>
T NetworkMessage::get()
{
	if(!canRead(sizeof(T)))
		return 0;

	T ret;
	memcpy(&ret, buffer + position, sizeof(T));
	position += sizeof(T);
}

template<typename T>
bool NetworkMessage::add(T value)
{
	if (!canAdd(sizeof(T)))
		return false;

	memcpy(buffer + position, &value, sizeof(T));
	position += sizeof(T);
	length += sizeof(T);
	return true;
}
#include <string>
#include "networkmessage.h"

uint8_t NetworkMessage::getByte()
{
	if (!canRead(1))
		return 0;
	return buffer[position++];
}

std::string NetworkMessage::getString()
{
	auto size = get<uint16_t>();
	if (!canRead(size))
		return {};

	char* content = reinterpret_cast<char*>(buffer) + position;
	position += size;
	return std::string(content, size);
}

bool NetworkMessage::addByte(uint8_t value)
{
	if (!canAdd(1))
		return false;
	buffer[position++] = value;
	length++;
	return true;
}

bool NetworkMessage::addString(const std::string& value)
{
	auto size = value.length();
	if (!canAdd(size + sizeof(uint16_t) || size > USHRT_MAX))
		return false;
	add<uint16_t>(size);
	memcpy(buffer + position, value.c_str(), size);
	position += size;
	length += position;
}
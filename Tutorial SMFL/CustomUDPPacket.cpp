#include "CustomUDPPacket.h"

CustomUDPPacket::CustomUDPPacket(UdpPacketType udpType, PacketType type)
{
	this->type = type;
	this->udpType = udpType;

	std::memcpy(buffer + bufferSize, &udpType, sizeof(udpType));
	bufferSize += sizeof(udpType);

	std::memcpy(buffer + bufferSize,&type, sizeof(type));
	bufferSize += sizeof(type);
}

void CustomUDPPacket::ReadBuffer(char buffer[1024], size_t _bufferSize)
{
	size_t offset = 0;

	std::memcpy(&udpType, buffer + offset, sizeof(udpType));
	offset += sizeof(udpType);

	std::memcpy(&type, buffer + offset, sizeof(type));
	offset += sizeof(type);

	std::memcpy(buffer, buffer + offset, _bufferSize - offset);
	bufferSize = _bufferSize - offset;
}

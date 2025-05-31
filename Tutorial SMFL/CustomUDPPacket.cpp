#include "CustomUDPPacket.h"

CustomUDPPacket::CustomUDPPacket(UdpPacketType udpType, PacketType type)
{
	this->type = type;
	this->udpType = udpType;

	WriteVariable(udpType);
	WriteVariable(type);
}

void CustomUDPPacket::ReadBuffer(const char* inputBuffer, size_t _bufferSize)
{
	size_t offset = 0;

	// Copiamos todo el paquete al buffer interno, incluyendo cabecera y payload
	std::memcpy(buffer, inputBuffer, _bufferSize);
	bufferSize = _bufferSize;

	ReadVariable(udpType, offset);
	ReadVariable(type, offset);

	payloadOffset = offset;
}

bool CustomUDPPacket::WriteString(const std::string& str)
{
	uint16_t length = static_cast<uint16_t>(str.size());
	if (!WriteVariable(length))
		return false;

	if (bufferSize + length > sizeof(buffer))
		return false;

	std::memcpy(buffer + bufferSize, str.data(), length);
	bufferSize += length;

	return true;
}

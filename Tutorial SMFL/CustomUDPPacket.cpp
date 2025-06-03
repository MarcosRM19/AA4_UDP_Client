#include "CustomUDPPacket.h"
#include <iostream>

CustomUDPPacket::CustomUDPPacket(UdpPacketType udpType, PacketType type, int playerId)
{
    bufferSize = 0;
    payloadOffset = 0;

    this->type = type;
    this->udpType = udpType;
    this->playerId = playerId;

    WriteVariable(static_cast<uint8_t>(udpType));
    WriteVariable(static_cast<uint8_t>(type));
    WriteVariable(playerId);

    uint8_t _udpType = 0;
    uint8_t _type = 0;
    size_t a = 0;

    ReadVariable(_udpType, a);
    ReadVariable(_type, a);
   /* std::cout << "El udpType es: " << static_cast<int>(_udpType) << std::endl;
    std::cout << "El type es: " << static_cast<int>(type) << std::endl;*/

    payloadOffset = bufferSize;
}

void CustomUDPPacket::ReadBuffer(const char* inputBuffer, size_t _bufferSize)
{
    size_t offset = 0;

    // Copiamos todo el paquete al buffer interno, incluyendo cabecera y payload
    std::memcpy(buffer, inputBuffer, _bufferSize);
    bufferSize = _bufferSize;

    uint8_t _udpType = 0;
    uint8_t _type = 0;

    ReadVariable(_udpType, offset);
    ReadVariable(_type, offset);
    ReadVariable(playerId, offset);

    udpType = static_cast<UdpPacketType>(_udpType);
    type = static_cast<PacketType>(_type);

    payloadOffset = offset;
}
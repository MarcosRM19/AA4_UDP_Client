#pragma once
#include <SFML/Network.hpp>
#include "PacketType.h"
#include "UDPPacketType.h"

class CustomUDPPacket
{
public:
    CustomUDPPacket() = default;
    CustomUDPPacket(UdpPacketType udpType, PacketType type);

    void ReadBuffer(char buffer[1024], size_t _bufferSize);

    PacketType type;
    UdpPacketType udpType;

    char buffer[1024];
    size_t bufferSize;
};

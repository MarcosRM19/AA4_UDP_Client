#pragma once
#include <SFML/Network.hpp>
#include "PacketType.h"

class CustomTCPPacket
{
public:

    CustomTCPPacket() = default;
    CustomTCPPacket(PacketType type);

    sf::Packet packet;
    PacketType type;
};

sf::Packet& operator >>(sf::Packet& packet, PacketType& type);
sf::Packet& operator <<(sf::Packet& packet, const PacketType& type);


#pragma once
#include "CustomPacket.h"
#include "CustomUDPPacket.h"
#include "Client.h"

#define PACKET_MANAGER PacketManager::Instance()
const int NUM_PLAYERS = 4;

class PacketManager
{
private:
    PacketManager() = default;
    PacketManager(const PacketManager&) = delete;
    PacketManager& operator=(const PacketManager&) = delete;

    void HandleHandshake(sf::Packet& packet);

    void HandleTest(sf::Packet& packet);


public:
    inline static PacketManager& Instance()
    {
        static PacketManager manager;
        return manager;
    }

    void Init();

    void SendHandshake(const std::string guid);
    void ProcessTCPReceivedPacket(CustomTCPPacket& customPacket);
    void ProcessUDPReceivedPacket(CustomUDPPacket& customPacket);

    void SendPacketToUDPServer(CustomUDPPacket& responsePacket);
    void SendPacketToTCPServer(CustomTCPPacket& responsePacket);
};


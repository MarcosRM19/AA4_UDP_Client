#pragma once
#include "CustomPacket.h"
#include "CustomUDPPacket.h"
#include "Client.h"

#define PACKET_MANAGER PacketManager::Instance()

class PacketManager
{
private:
    PacketManager() = default;
    PacketManager(const PacketManager&) = delete;
    PacketManager& operator=(const PacketManager&) = delete;

    void HandleHandshake(sf::Packet& packet);

    void HandleTest(sf::Packet& packet);

    std::vector<CustomUDPPacket> criticsPacketsServer;
    std::vector<CustomUDPPacket> criticsPacketsClient;

    int globalIdPlayer;
    int localIdPlayer;

public:
    inline static PacketManager& Instance()
    {
        static PacketManager manager;
        return manager;
    }

    void Init();
    int GetCriticId(CustomUDPPacket packet);

    void SendHandshake(const std::string guid);
    void ProcessTCPReceivedPacket(CustomTCPPacket& customPacket);
    void ProcessUDPReceivedPacket(CustomUDPPacket& customPacket);

    void SendPacketToUDPServer(CustomUDPPacket& responsePacket);
    void SendPacketToTCPServer(CustomTCPPacket& responsePacket);

    void SendCriticsPackets();
    void SendPingPackets();

    inline void AddCriticalPacketClient(CustomUDPPacket customPacket) { criticsPacketsClient.push_back(customPacket); }

    inline int GetGlobalId() { return globalIdPlayer; }
};


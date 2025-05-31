#pragma once
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include "PacketType.h"
#include "PacketManager.h"

#define EVENT_MANAGER EventManager::Instance()

class EventManager
{
public:
    using TCPCallback = std::function<void(CustomTCPPacket&)>;
    using UDPCallback = std::function<void(CustomUDPPacket&)>;


    void TCPSubscribe(const PacketType type, TCPCallback callback);
    void UDPSubscribe(const PacketType type, UDPCallback callback);

    void TCPEmit(const PacketType type, CustomTCPPacket customPacket);
    void UDPEmit(const PacketType type, CustomUDPPacket customPacket);

    static EventManager& Instance();

private:

    std::unordered_map<PacketType, std::vector<TCPCallback>> TCPsubscribers;
    std::unordered_map<PacketType, std::vector<UDPCallback>> UDPsubscribers;

    EventManager() = default;
    EventManager(const EventManager&) = delete;
    EventManager& operator=(const EventManager&) = delete;
};


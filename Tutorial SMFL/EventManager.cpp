#include "EventManager.h"
#include <iostream>

void EventManager::TCPSubscribe(const PacketType type, TCPCallback _callback)
{
    TCPsubscribers[type].push_back(_callback);
}

void EventManager::UDPSubscribe(const PacketType type, UDPCallback callback)
{
    UDPsubscribers[type].push_back(callback);
}

void EventManager::TCPEmit(const PacketType type, CustomTCPPacket customPacket)
{

    std::unordered_map<PacketType, std::vector<TCPCallback>>::iterator it = TCPsubscribers.find(type);

    if (it != TCPsubscribers.end()) {
        for (TCPCallback& callback : it->second) {
            callback(customPacket);
        }
    }
    else
        std::cerr << "The event: " << type << " does not exists" << std::endl;
}

void EventManager::UDPEmit(const PacketType type, CustomUDPPacket customPacket)
{
    std::unordered_map<PacketType, std::vector<UDPCallback>>::iterator it = UDPsubscribers.find(type);

    if (it != UDPsubscribers.end()) {
        for (UDPCallback& callback : it->second) {
            callback(customPacket);
        }
    }
    else
        std::cerr << "The event: " << type << " does not exists" << std::endl;
}

EventManager& EventManager::Instance()
{
    static EventManager instance;
    return instance;
}

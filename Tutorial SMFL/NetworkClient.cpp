#include "NetworkClient.h"
#include "CustomPacket.h"
#include <iostream>
#include "EventManager.h"
#include "PacketManager.h"

NetworkClient::NetworkClient(const std::string& ip, int port, const std::string& guid)
	: ip(ip), port(port), guid(guid), socket(std::make_shared<sf::TcpSocket>()) { }

void NetworkClient::HandleIncomingPackets()
{
    CustomTCPPacket customPacket;

    sf::Socket::Status status = socket->receive(customPacket.packet);

    if (status == sf::Socket::Status::Done) {
        std::cout << "Voy a procesar el paquete" << std::endl;
        PACKET_MANAGER.ProcessTCPReceivedPacket(customPacket);
    }
    else if (status == sf::Socket::Status::Disconnected)
    {
        EVENT_MANAGER.TCPEmit(DISCONNECT, customPacket);
    }
}

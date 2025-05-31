#include "Client.h"

#include "PacketManager.h"
#include "EventManager.h"
#include "PacketType.h"
#include "SceneManager.h"
#include "GameManager.h"
#include <iostream>

Client::Client(const NetworkClient& network, const Player& player)
	: networkClient(network), playerData(player) {}

void Client::Update(sf::RenderWindow& window)
{
    
}

void Client::HandleEvent(const sf::Event& event, sf::RenderWindow& window)
{
    
}


void Client::HandleIncomingPackets()
{
	networkClient.HandleIncomingPackets();
}


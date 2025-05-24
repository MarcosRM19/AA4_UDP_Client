#pragma once
#include <string>
#include <SFML/Network.hpp>
#include "ClientState.h"
#include "NetworkClient.h"
#include "PlayerData.h"

class Client
{
private:

    NetworkClient networkClient;
    PlayerData playerData;

public:

    Client(const NetworkClient& network, const PlayerData& player);

    void Update(sf::RenderWindow& window);
    void HandleEvent(const sf::Event& event, sf::RenderWindow& window);

    void HandleIncomingPackets();

    NetworkClient& GetNetwork() { return networkClient; }
    PlayerData& GetPlayerData() { return playerData; }

   
};


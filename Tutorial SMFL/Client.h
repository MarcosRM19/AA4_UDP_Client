#pragma once
#include <string>
#include <SFML/Network.hpp>
#include "ClientState.h"
#include "NetworkClient.h"
#include "Player.h"

class Client
{
private:

    NetworkClient networkClient;
    Player playerData;

public:

    Client(const NetworkClient& network, const Player& player);

    void Update(sf::RenderWindow& window);
    void HandleEvent(const sf::Event& event, sf::RenderWindow& window);

    void HandleIncomingPackets();

    NetworkClient& GetNetwork() { return networkClient; }
    Player& GetPlayerData() { return playerData; }

   
};


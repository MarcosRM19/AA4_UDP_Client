#include "GameManager.h"
#include "TimeManager.h"
#include <iostream>
#include "NetworkManager.h"
#include "EventManager.h"

void GameManager::Init(sf::RenderWindow& _window)
{
	
}

void GameManager::Update(sf::RenderWindow& window, const sf::Event& event)
{
	

	window.display();
}

void GameManager::HandleEvent(const sf::Event& event, sf::RenderWindow& window)
{
	if (event.is<sf::Event::Closed>())
	{
		window.close();
		CustomPacket customPacket(DISCONNECT);
		EVENT_MANAGER.Emit(DISCONNECT, customPacket);
	}
}

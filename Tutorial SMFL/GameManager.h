#pragma once
#include "Client.h"

#define GAME GameManager::Instance()

class GameManager
{
private:
	std::vector<std::shared_ptr<Client>> clients;

	GameManager() = default;
	GameManager(const GameManager&) = delete;
	GameManager& operator =(const GameManager&) = delete;

public:
	inline static GameManager& Instance()
	{
		static GameManager manager;
		return manager;
	}

	void Init(sf::RenderWindow& _window);
	void Update(sf::RenderWindow& window, const sf::Event& event);

	void HandleEvent(const sf::Event& event, sf::RenderWindow& window);

};


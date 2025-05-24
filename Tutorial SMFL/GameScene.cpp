#include "GameScene.h"
#include "GameManager.h"

void GameScene::Enter(sf::RenderWindow& window)
{
	std::cout << "Enter Game Scene" << std::endl;
}

void GameScene::Exit()
{
	std::cout << "Exit Game Scene" << std::endl;
}

void GameScene::Update(sf::RenderWindow& window, const sf::Event& event)
{
	GAME.Update(window, event);
}

void GameScene::DetectRectangle(sf::Vector2f mousePosition)
{
}

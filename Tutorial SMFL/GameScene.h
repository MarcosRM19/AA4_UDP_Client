#pragma once
#include "Scene.h"
#include "Player.h"

class GameScene : public Scene
{
private:
    std::vector<std::unique_ptr<Player>> players;

    sf::Clock clock;

    std::vector<sf::FloatRect> collisionRects;
    std::vector<int> tileData;
    int mapWidth = 25;
    int mapHeight = 15;
    int tileSize = 32;

    void GenerateColisions();

    void InitPlayers();

public:
    void Enter(sf::RenderWindow& window) override;
    void Exit() override;
    void Update() override;
    void Render(sf::RenderWindow& window) override;
    void HandleEvent(const sf::Event& event) override;
    void DetectRectangle(sf::Vector2f mousePosition) override;
};


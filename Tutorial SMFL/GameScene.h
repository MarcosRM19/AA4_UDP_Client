#pragma once
#include "Scene.h"

class GameScene : public Scene
{
public:
    void Enter(sf::RenderWindow& window) override;
    void Exit() override;
    void Update(sf::RenderWindow& window, const sf::Event& event) override;
    void DetectRectangle(sf::Vector2f mousePosition) override;
};


#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

class Scene
{
protected:
    sf::Text* currentText;
    std::string inputText;
    const std::size_t maxLengthName = 12;
    const std::size_t maxLengthPasword = 12;

public:
    Scene();  
    virtual ~Scene();
    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;

    virtual void Enter(sf::RenderWindow& window) = 0;
    virtual void Exit() = 0;
    virtual void Update() = 0;
    virtual void Render(sf::RenderWindow& window) = 0;
    virtual void HandleEvent(const sf::Event& event);
    virtual void DetectRectangle(sf::Vector2f mousePosition) = 0;
    virtual void SetCurrentPlayer(int id) = 0;

};

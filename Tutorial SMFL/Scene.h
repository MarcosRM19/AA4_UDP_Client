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
    const std::size_t idMax = 5;

public:
    Scene();  
    virtual ~Scene();
    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;

    virtual void Enter(sf::RenderWindow& window) = 0;
    virtual void Exit() = 0;
    virtual void Update(sf::RenderWindow& window, const sf::Event& event) = 0;
    virtual void HandleEvent(sf::RenderWindow& window, const sf::Event& event);
    virtual void DetectRectangle(sf::Vector2f mousePosition) = 0;

};

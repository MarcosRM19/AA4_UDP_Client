#pragma once
#include <SFML/Graphics.hpp>

class PlayerData
{
private:
    sf::Vector2f position;
    sf::Vector2f velocity;

    float moveSpeed;
    float jumpSpeed;
    float gravity = 900.f; // pixeles por segundo^2
    bool isOnGround = false;

    sf::RectangleShape shape;

public:
    PlayerData(sf::Vector2f startPosition, float width, float height, sf::Color color);

    void Update(float deltaTime);
    void Draw(sf::RenderWindow& window);

    void HandleEvent(const sf::Event& event, sf::RenderWindow& window);
    void ApplyGravity(float deltaTime);
    void CheckGroundCollision(float groundY);

    inline sf::Vector2f GetPosition() const { return position; }

    inline void SetPosition(sf::Vector2f newPosition) { position = newPosition; }
};


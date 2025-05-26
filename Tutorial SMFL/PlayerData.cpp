#include "PlayerData.h"
#include <iostream>

PlayerData::PlayerData(sf::Vector2f startPosition, float width, float height, sf::Color color)
{

}

void PlayerData::ApplyGravity(float deltaTime)
{
    velocity.y += gravity * deltaTime;
}

void PlayerData::CheckGroundCollision(float groundY)
{
    if (position.y >= groundY)
    {
        position.y = groundY;
        velocity.y = 0.f;
        isOnGround = true;
    }
}

void PlayerData::Update(float deltaTime)
{
    HandleEvent();
    ApplyGravity(deltaTime);

    position += velocity * deltaTime;

    // Ejemplo: colisión simple con el suelo
    CheckGroundCollision(500.f); //altura del suelo

    shape.setPosition(position);
}

void PlayerData::Draw(sf::RenderWindow& window)
{
    window.draw(shape);
}

void PlayerData::HandleEvent(const sf::Event& event, sf::RenderWindow& window)
{
    velocity.x = 0.f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        velocity.x = -moveSpeed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        velocity.x = moveSpeed;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && isOnGround)
    {
        velocity.y = -jumpSpeed;
        isOnGround = false;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        std::cout << "Interact button pressed\n";
        // Aquí puedes lanzar un evento de interacción
    }
}

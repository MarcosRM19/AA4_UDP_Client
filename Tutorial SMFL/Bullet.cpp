#include "Bullet.h"
#include "GameManager.h" // Para detectar colisiones con el mapa

Bullet::Bullet(const sf::Vector2f& startPos, const sf::Vector2f& direction)
    : isActive(true), lifeTime(0.f), maxLifeTime(3.f)
{
    shape.setSize({ 8.f, 4.f });
    shape.setFillColor(sf::Color::Yellow);

    shape.setPosition(startPos);

    speed = 500.f;
    velocity = direction.normalized() * speed;
}

void Bullet::Update(float deltaTime)
{
    if (!isActive)
        return;

    sf::Vector2f nextPos = shape.getPosition() + velocity * deltaTime;
    sf::FloatRect nextBounds(nextPos, shape.getSize());

    if (GAME.CollidesWithMap(nextBounds))
    {
        isActive = false;
        return;
    }

    shape.setPosition(nextPos);

    lifeTime += deltaTime;
    if (lifeTime > maxLifeTime)
    {
        isActive = false;
    }
}

void Bullet::Render(sf::RenderWindow& window) const
{
    if (isActive)
        window.draw(shape);
}

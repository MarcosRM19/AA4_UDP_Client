#pragma once
#include <SFML/Graphics.hpp>

class Bullet
{
private:
    sf::RectangleShape shape;
    sf::Vector2f velocity;
    bool isActive;

    float speed;
    float lifeTime;                 
    const float maxLifeTime;

public:
    Bullet(const sf::Vector2f& startPos, const sf::Vector2f& direction);

    void Update(float deltaTime);
    void Render(sf::RenderWindow& window) const;

    inline bool GetIsActive() const { return isActive; }
    inline sf::FloatRect GetBounds() const { return shape.getGlobalBounds(); }
};

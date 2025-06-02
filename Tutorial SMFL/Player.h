#pragma once
#include <SFML/Graphics.hpp>
#include <functional>

class Player
{
private:
    sf::Vector2f position;
    sf::Vector2f velocity;

    float moveSpeed;
    float jumpForce;
    float gravity;
    bool isOnGround;

    float initialHealth;
    float health;
    float lives;

    float shootCooldown;
    float shootTimer;

    bool movingLeft;
    bool movingRight;
    bool jumpRequested;
    bool shootRequested;

    bool facingRight;

    sf::Texture texture;
    std::shared_ptr<sf::Sprite> sprite;

    std::function<void(const sf::Vector2f&, const sf::Vector2f&)> shootCallback;
    void Shoot();
    void Respawn();

public:
    Player(sf::Vector2f startPosition, sf::Color color);

    void SetShootCallback(std::function<void(const sf::Vector2f&, const sf::Vector2f&)> callback);

    void HandleEvent(const sf::Event& event);
    void PrepareMovement(float deltaTime);
    void ApplyMovement(float deltaTime);
    void Update(float deltaTime);
    void Render(sf::RenderWindow& window);
    void ReceiveDamage();

    inline void MoveHorizontally(float moveX) { position.x += moveX; sprite->setPosition(position); }
    inline void MoveVertically(float moveY) { position.y += moveY; sprite->setPosition(position); }

    sf::FloatRect GetNextBounds(float deltaTime) const;
    inline sf::FloatRect GetGlobalBounds() const { return sprite->getGlobalBounds(); }
    inline sf::Vector2f GetVelocity() const { return velocity; }
    inline sf::Vector2f GetPosition() const { return position; }
    inline sf::Vector2f GetSize() const { return sprite->getLocalBounds().size; }
    
    inline void StopVertical() { velocity.y = 0.f; }
    inline void SetIsOnGround(bool _isOnGround) { isOnGround = _isOnGround; }
};


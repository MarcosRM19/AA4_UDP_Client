#pragma once
#include "Bullet.h"
#include "CustomUDPPacket.h"

const sf::Time interval = sf::seconds(0.2f);

class Player
{
private:
    sf::Vector2f position;
    sf::Vector2f velocity;

    float moveSpeed;
    float jumpForce;
    float gravity;
    bool isOnGround;

    std::vector<std::shared_ptr<Bullet>> bullets;
    float shootCooldown;
    float shootTimer;

    bool movingLeft;
    bool movingRight;
    bool jumpRequested;
    bool shootRequested;

    bool facingRight;

    sf::RectangleShape shape;

    void Shoot();

    int idMovement = 0;
    int idCritic = 0;
    int idPlayer;
    std::vector<CustomUDPPacket> positionsPackets;
    sf::Clock sendPositionClock;

public:
    Player(sf::Vector2f startPosition, sf::Color color);

    void HandleEvent(const sf::Event& event);
    void PrepareMovement(float deltaTime);
    void ApplyMovement(float deltaTime);
    void Update(float deltaTime);
    void Render(sf::RenderWindow& window);

    inline void MoveHorizontally(float moveX) { position.x += moveX; shape.setPosition(position); }
    inline void MoveVertically(float moveY) { position.y += moveY; shape.setPosition(position); }

    void SendPosition();
    void BacktToValidPosition(int id);
    void ResetPositionsPackets();

    sf::FloatRect GetNextBounds(float deltaTime) const;
    inline sf::Vector2f GetVelocity() { return velocity; }
    inline sf::Vector2f GetPosition() const { return position; }
    inline sf::Vector2f GetVelocity() const { return velocity; }
    inline sf::Vector2f GetSize() const { return shape.getSize(); }
    inline const std::vector<std::shared_ptr<Bullet>>& GetBullets() const { return bullets; }
    inline const int GetIdPlayer() { return idPlayer; }
    inline int GetIdCritic() { return idCritic; }
    
    inline void StopVertical() { velocity.y = 0.f; }
    inline void SetColor(sf::Color color) { shape.setFillColor(color); }
    inline void SetIsOnGround(bool _isOnGround) { isOnGround = _isOnGround; }
    inline void SetId(int id) { idPlayer = id; }
    inline void AddIdCritic() { idCritic++; }

};


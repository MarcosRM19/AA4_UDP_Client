#pragma once
#include "CustomUDPPacket.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <functional>

const sf::Time interval = sf::seconds(0.05f);
const sf::Time interpolationTime = sf::seconds(0.1f);

struct ValidPackets
{
    sf::Vector2f position;
    int id;

    ValidPackets() = default;
    ValidPackets(sf::Vector2f position, int id) : position(position), id(id) {};
};

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
    bool mockeryRequested;

    std::vector<ValidPackets> enemyPositions;
    sf::Clock elapsedTime;
    sf::Clock interpolationTimer;
    float totalTime;
    
    bool isAlive;

    bool facingRight;

    sf::Texture texture;
    std::shared_ptr<sf::Sprite> sprite;
    float scale;

    std::function<void(const sf::Vector2f&, const sf::Vector2f&)> shootCallback;
    void Mock();
    void Shoot();
    void Respawn();

    int idMovement = 0;
    int idCritic = 0;
    int idPlayer;
    std::vector<CustomUDPPacket> positionsPackets;
    sf::Clock sendPositionClock;

    sf::Sound* mockery;
    sf::SoundBuffer mockeryBuffer;

    bool startInterpolate;

public:
    Player(sf::Vector2f startPosition, sf::Color color);

    void SetShootCallback(std::function<void(const sf::Vector2f&, const sf::Vector2f&)> callback);

    void HandleEvent(const sf::Event& event);
    void PrepareMovement(float deltaTime);
    void Update(float deltaTime);
    void UpdateEnemy(float deltaTime);
    void Render(sf::RenderWindow& window);
    void ReceiveDamage();

    inline void MoveHorizontally(float moveX) { position.x += moveX; sprite->setPosition(position); }
    inline void MoveVertically(float moveY) { position.y += moveY; sprite->setPosition(position); }

    void SendPosition();
    void BacktToValidPosition(int id);
    void ResetPositionsPackets();

    void SentCriticPacket(PacketType type);
    void AddEnemyPosition(sf::Vector2f newPosition, int id);
    void CheckIsDead(int _lives);

    sf::FloatRect GetNextBounds(float deltaTime) const;
    inline sf::FloatRect GetGlobalBounds() const { return sprite->getGlobalBounds(); }
    inline sf::Vector2f GetVelocity() const { return velocity; }
    inline sf::Vector2f GetPosition() const { return position; }
    inline sf::Vector2f GetSize() const { return sprite->getGlobalBounds().size; }
    inline const int GetIdPlayer() { return idPlayer; }
    inline int GetIdCritic() { return idCritic; }
    inline std::vector<ValidPackets> GetEnemyPosition() {return enemyPositions; }
    
    inline void StopVertical() { velocity.y = 0.f; }
    inline void SetColor(sf::Color color) { sprite->setColor(color); }
    inline void SetIsOnGround(bool _isOnGround) { isOnGround = _isOnGround; }
    inline void SetShootRequested(bool _shootRequested) { shootRequested = _shootRequested; }
    inline void SetMockeryRequested(bool _mockeryRequested) { mockeryRequested = _mockeryRequested; }
    inline void SetId(int id) { idPlayer = id; }
    inline void SetStartInterpolate(bool state) { startInterpolate = state; }

    inline void AddIdCritic() { idCritic++; }
    inline void RestartElapsedTime() { elapsedTime.restart(); }

    sf::Vector2f Lerp(const sf::Vector2f& start, const sf::Vector2f& end, float t);
};


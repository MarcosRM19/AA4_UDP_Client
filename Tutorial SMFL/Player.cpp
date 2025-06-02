#include "Player.h"
#include "GameManager.h"
#include <iostream>
#include "EventManager.h"

Player::Player(sf::Vector2f startPosition, sf::Color color)
    : position(startPosition)
{
    moveSpeed = 100.f;
    jumpForce = 350.f;
    gravity = 500.0f;
    isOnGround = false;

    initialHealth = 5;
    health = initialHealth;
    lives = 3;

    shootCooldown = 0.5f;
    shootTimer = 0.f;

    movingLeft = false;
    movingRight = false;
    jumpRequested = false;
    shootRequested = false;

    facingRight = true;

    if (!texture.loadFromFile("player.png"))
        std::cerr << "Error loading the texture of the player" << std::endl;
    sprite = std::make_shared<sf::Sprite>(texture);
    sprite->setPosition(position);
}

void Player::SetShootCallback(std::function<void(const sf::Vector2f&, const sf::Vector2f&)> callback)
{
    shootCallback = callback;  // Guarda la funci�n que ser� llamada cuando el jugador dispare
}

void Player::HandleEvent(const sf::Event& event)
{
    if (const sf::Event::KeyPressed* keyPressed = event.getIf<sf::Event::KeyPressed>())
    {
        if (keyPressed->code == sf::Keyboard::Key::Left)
            movingLeft = true;
        else if (keyPressed->code == sf::Keyboard::Key::Right)
            movingRight = true;
        else if (keyPressed->code == sf::Keyboard::Key::Up && isOnGround)
            jumpRequested = true;
        else if (keyPressed->code == sf::Keyboard::Key::Space)
            shootRequested = true;
    }
    else if (const sf::Event::KeyReleased* keyReleased = event.getIf<sf::Event::KeyReleased>())
    {
        if (keyReleased->code == sf::Keyboard::Key::Left)
            movingLeft = false;
        else if (keyReleased->code == sf::Keyboard::Key::Right)
            movingRight = false;
        else if (keyPressed->code == sf::Keyboard::Key::Space)
            shootRequested = false;
    }
}

void Player::PrepareMovement(float deltaTime)
{
    velocity.x = 0.f;

    if (movingLeft)
    {
        velocity.x -= moveSpeed;
        facingRight = false;
    }

    if (movingRight)
    {
        velocity.x += moveSpeed;
        facingRight = true;
    }

    if (shootRequested)
    {
        Shoot();
    }

    if (jumpRequested && isOnGround)
    {
        velocity.y = -jumpForce;
        jumpRequested = false;
        isOnGround = false;
    }

    if(!isOnGround)
        velocity.y += gravity * deltaTime;
}

void Player::ApplyMovement(float deltaTime)
{
    position += velocity * deltaTime;
    sprite->setPosition(position);
}

void Player::Update(float deltaTime)
{
    if (shootTimer > 0.f)
        shootTimer -= deltaTime;
}

void Player::Render(sf::RenderWindow& window)
{
    window.draw(*sprite);
}

void Player::Shoot()
{
    if (shootTimer > 0.f)
        return;

    shootTimer = shootCooldown;

    float offsetX = facingRight ? GetSize().x : 0.f;
    sf::Vector2f bulletPos = position + sf::Vector2f(offsetX, GetSize().y / 2.f);
    sf::Vector2f bulletDir = facingRight ? sf::Vector2f(1.f, 0.f) : sf::Vector2f(-1.f, 0.f);

    if (shootCallback)
    {
        shootCallback(bulletPos, bulletDir);  // Notifica a GameScene para crear la bala
    }
}

void Player::ReceiveDamage()
{
    if (health > 1)
    {
        health--;
        return;
    }

    if (lives > 1)
    {
        lives--;
        health = initialHealth;
        Respawn();
        return;
    }

    std::cout << "YOU LOSE";
}

void Player::SendPosition()
{
    if (sendPositionClock.getElapsedTime() >= interval)
    {
        //CustomUDPPacket customPacket(UdpPacketType::NORMAL, SEND_POSITION, PACKET_MANAGER.GetGlobalId());

        //customPacket.WriteVariable(idMovement);
        //customPacket.WriteVariable(position.x);
        //customPacket.WriteVariable(position.y);

        //std::cout<< "ID: "<< idMovement << ", Position X: " << position.x << ", Position Y: " << position.y << std::endl;

        //positionsPackets.push_back(customPacket);
        //EVENT_MANAGER.UDPEmit(customPacket.type, customPacket);
        //idMovement++;
        //sendPositionClock.restart();
    }
}

void Player::Respawn()
{
    position = GAME.GetSpawnPositions()[0];
    sprite->setPosition(position);
}
sf::FloatRect Player::GetNextBounds(float deltaTime) const
{
    sf::Vector2f nextPos = position + velocity * deltaTime;
    return sf::FloatRect(nextPos, GetSize());
}

void Player::BacktToValidPosition(int id)
{
    for (int i = 0; i < positionsPackets.size(); i++)
    {
        int _id = 0;
        size_t size = positionsPackets[i].payloadOffset;
        if (positionsPackets[i].ReadVariable(_id, size) == id)
        {
            sf::Vector2f position;
            positionsPackets[i].ReadVariable(position.x, size);
            positionsPackets[i].ReadVariable(position.y, size);

            //mover al player a la posicion de position
        }
    }

    ResetPositionsPackets();
}

void Player::ResetPositionsPackets()
{
    positionsPackets.clear();
}

#include "Player.h"
#include <iostream>
#include "EventManager.h"

Player::Player(sf::Vector2f startPosition, sf::Color color)
{
    position = startPosition;

    moveSpeed = 100.f;
    jumpForce = 350.f;
    gravity = 500.0f;
    isOnGround = false;

    shootCooldown = 0.5f;
    shootTimer = 0.f;

    movingLeft = false;
    movingRight = false;
    jumpRequested = false;
    shootRequested = false;

    facingRight = true;

    float width = 25.0f;
    float height = 50.0f;

    shape.setSize(sf::Vector2f(width, height));
    shape.setFillColor(color);
    shape.setPosition(position);
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
    shape.setPosition(position);
}

void Player::Update(float deltaTime)
{
    if (shootTimer > 0.f)
        shootTimer -= deltaTime;

    // Tiene que ser asi por si se borran balas durante el for
    // Este codigo es de chatgpt
    for (auto it = bullets.begin(); it != bullets.end(); )
    {
        (*it)->Update(deltaTime);
        if (!(*it)->GetIsActive())
            it = bullets.erase(it);
        else
            ++it;
    }

    shape.setPosition(position);
    SendPosition();
}

void Player::Shoot()
{
    if (shootTimer > 0.f)
        return;

    shootRequested = false;

    shootTimer = shootCooldown;

    float offsetX = facingRight ? shape.getSize().x : 0.f;
    sf::Vector2f bulletPos = position + sf::Vector2f(offsetX, shape.getSize().y / 2.f);
    sf::Vector2f bulletDir = facingRight ? sf::Vector2f(1.f, 0.f) : sf::Vector2f(-1.f, 0.f);

    std::shared_ptr<Bullet> newBullet = std::make_shared<Bullet>(bulletPos, bulletDir);
    bullets.push_back(newBullet);
}

void Player::Render(sf::RenderWindow& window)
{
    window.draw(shape);

    for (const std::shared_ptr<Bullet>& bullet : bullets)
    {
        bullet->Render(window);
    }
}

void Player::SendPosition()
{
    if (sendPositionClock.getElapsedTime() >= interval)
    {
        CustomUDPPacket customPacket(UdpPacketType::NORMAL, SEND_POSITION);

        customPacket.WriteVariable(id);
        customPacket.WriteVariable(position.x);
        customPacket.WriteVariable(position.y);

        std::cout << "Position X: " << position.x << ", Position Y: " << position.y << std::endl;

        positionsPackets.push_back(customPacket);
        EVENT_MANAGER.UDPEmit(customPacket.type, customPacket);

        sendPositionClock.restart();
    }
}

sf::FloatRect Player::GetNextBounds(float deltaTime) const
{
    sf::Vector2f nextPos = position + velocity * deltaTime;
    return sf::FloatRect(nextPos, shape.getSize());
}
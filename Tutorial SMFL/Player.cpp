#include "Player.h"
#include "GameManager.h"
#include <iostream>
#include "EventManager.h"

Player::Player(sf::Vector2f startPosition, sf::Color color)
    : position(startPosition)
{
    moveSpeed = 100.f;
    jumpForce = 350.f;
    gravity = 500.f;
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
    mockeryRequested = false;

    facingRight = true;

    if (!texture.loadFromFile("../Assets/Sprites/Duck.png"))
        std::cerr << "Error loading the texture of the player" << std::endl;
    sprite = std::make_shared<sf::Sprite>(texture);
    sprite->setPosition(position);

    sprite->setOrigin(sf::Vector2f(GetSize().x / 2, 0));
    scale = 0.25f;
    sprite->setScale({ scale, scale });

    if (!mockeryBuffer.loadFromFile("../Assets/Sounds/Quack.wav")) {
        std::cerr << "Error loading the audio of the player" << std::endl;
    }

    mockery = new sf::Sound(mockeryBuffer);

    startInterpolate = false;
    totalTime = 1;
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
        {
            if(!shootRequested)
                SentCriticPacket(SEND_START_SHOOT);
            shootRequested = true;
        }
        else if (keyPressed->code == sf::Keyboard::Key::E)
        {
            if(!mockeryRequested)
                SentCriticPacket(SEND_MOCKERY);
            mockeryRequested = true;

        }
    }
    else if (const sf::Event::KeyReleased* keyReleased = event.getIf<sf::Event::KeyReleased>())
    {
        if (keyReleased->code == sf::Keyboard::Key::Left)
            movingLeft = false;
        else if (keyReleased->code == sf::Keyboard::Key::Right)
            movingRight = false;
        else if (keyReleased->code == sf::Keyboard::Key::Space)
        {
            if(shootRequested)
                SentCriticPacket(SEND_STOP_SHOOT);
            shootRequested = false;
        }

    }
}

void Player::PrepareMovement(float deltaTime)
{
    velocity.x = 0.f;

    if (movingLeft)
    {
        sprite->setScale({ -1.f * scale, scale });
        velocity.x -= moveSpeed;
        facingRight = false;
    }

    if (movingRight)
    {
        sprite->setScale({ scale, scale });
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

void Player::Update(float deltaTime)
{
    if (shootTimer > 0.f)
        shootTimer -= deltaTime;

    if (mockeryRequested)
        Mock();

    sprite->setPosition(position);
    SendPosition();
}

void Player::UpdateEnemy(float deltaTime)
{
    if (shootTimer > 0.f)
        shootTimer -= deltaTime;
    
    if (shootRequested)
    {
        Shoot();
    }

    if (mockeryRequested)
    {
        Mock();
    }

    //este codigo es de chatgpt
    if (interpolationTimer.getElapsedTime() >= interpolationTime)
    {
        startInterpolate = true;
        interpolationTimer.restart(); 
        elapsedTime.restart();
    }

    if (startInterpolate)
    {
        if(enemyPositions.empty())
            return;

        std::vector<ValidPackets> validPackets;
        validPackets.push_back(enemyPositions[0]);
        for (int i = 1; i < enemyPositions.size(); i++)
        {
            if (enemyPositions[i].id < enemyPositions[i - 1].id)
                continue;

            validPackets.push_back(enemyPositions[i]);
        }

        float t = elapsedTime.getElapsedTime().asSeconds() / 0.45;
        float segmentLength = 1.f / (validPackets.size() - 1);
        size_t segmentIndex = static_cast<size_t>(t / segmentLength);

        if (segmentIndex < validPackets.size() - 1)
        {
            float normalizedT = (t - segmentIndex * segmentLength) / segmentLength;
            sprite->setPosition(Lerp(validPackets[segmentIndex].position, validPackets[segmentIndex + 1].position, normalizedT));
            position = Lerp(validPackets[segmentIndex].position, validPackets[segmentIndex + 1].position, normalizedT);
        }

        if (t >= 1.f)
        {
            elapsedTime.restart();
            enemyPositions.clear();
            startInterpolate = false;
        }
    }
}

void Player::Render(sf::RenderWindow& window)
{
    window.draw(*sprite);
}

void Player::Mock()
{
    mockeryRequested = false;
    mockery->play();
}

void Player::Shoot()
{
    if (shootTimer > 0.f)
        return;

    shootTimer = shootCooldown;

    float offsetX = facingRight ? GetSize().x : -GetSize().x;
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
        CustomUDPPacket customPacket(UdpPacketType::NORMAL, SEND_POSITION, PACKET_MANAGER.GetGlobalId());

        customPacket.WriteVariable(idMovement);
        customPacket.WriteVariable(position.x);
        customPacket.WriteVariable(position.y);

        //std::cout<< "ID: "<< idMovement << ", Position X: " << position.x << ", Position Y: " << position.y << std::endl;

        positionsPackets.push_back(customPacket);
        EVENT_MANAGER.UDPEmit(customPacket.type, customPacket);
        idMovement++;
        sendPositionClock.restart();
    }
}

void Player::Respawn()
{
    position = GAME.GetSpawnPositions()[2];
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
        positionsPackets[i].ReadVariable(_id, size);
     
        if (_id == id)
        {
            sf::Vector2f _position;
            positionsPackets[i].ReadVariable(_position.x, size);
            positionsPackets[i].ReadVariable(_position.y, size);
            position = _position;
            sprite->setPosition(_position);
        }
    }

    ResetPositionsPackets();
}

void Player::ResetPositionsPackets()
{
    positionsPackets.clear();
}

void Player::SentCriticPacket(PacketType type)
{
    CustomUDPPacket customPacket(UdpPacketType::CRITIC, type, PACKET_MANAGER.GetGlobalId());
    customPacket.WriteVariable(idCritic);
    idCritic++;

    //EVENT_MANAGER.UDPEmit(customPacket.type, customPacket);
}

void Player::AddEnemyPosition(sf::Vector2f newPosition, int id)
{
    enemyPositions.push_back({ newPosition, id });          
}

sf::Vector2f Player::Lerp(const sf::Vector2f& start, const sf::Vector2f& end, float t)
{
    return start + (end - start) * t;
}

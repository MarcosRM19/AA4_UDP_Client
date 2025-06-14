#pragma once
#include "Scene.h"
#include "Player.h"
#include "Bullet.h"
#include <functional>

class GameScene : public Scene
{
private:
    std::vector<std::shared_ptr<Player>> players;
    std::vector<std::shared_ptr<Bullet>> bullets;

    int playerId;

    sf::Clock clock;

    std::vector<sf::FloatRect> collisionRects;
    std::vector<int> tileData;
    int mapWidth = 25;
    int mapHeight = 15;
    int tileSize = 32;

    void InitPlayers();

    std::function<void(const sf::Vector2f&, const sf::Vector2f&)> shootCallback;

public:
    void Enter(sf::RenderWindow& window) override;
    void Exit() override;
    void UpdateReferencePlayer(float deltaTime);
    void UpdateEnemyPlayer(float deltaTime);
    void UpdateBullets(float deltaTime);
    void Update() override;
    void Render(sf::RenderWindow& window) override;
    void HandleEvent(const sf::Event& event) override;
    void DetectRectangle(sf::Vector2f mousePosition) override;
    void SetShootCallback(std::function<void(const sf::Vector2f&, const sf::Vector2f&)> callback);
    void CreateBullet(const sf::Vector2f& bulletPos, const sf::Vector2f& bulletDir);
    void SetCurrentPlayer(int id) override;
    void RecognizePlayer(int idReferencePlayer, int idEnemy);
};


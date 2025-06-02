#pragma once
#include "Client.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>

#define GAME GameManager::Instance()

class GameManager
{
private:
    std::vector<std::shared_ptr<Client>> clients;
    std::stringstream buffer;
    std::string json;
    int width;
    int height;
    int tileWidth;
    int tileHeight;
    sf::Texture tileset;
    std::vector<int> tileData;
    std::vector<sf::FloatRect> collisionRects;

    std::vector<sf::Vector2f> spawnPositions;

    GameManager() = default;
    GameManager(const GameManager&) = delete;
    GameManager& operator =(const GameManager&) = delete;

    int ExtractInt(const std::string& json, const std::string& key);
    std::vector<int> ExtractTileData(const std::string& json);
    
    void InitMap();

    Player* referencePlayer;
    Player* enemyPlayer;

public:
    inline static GameManager& Instance()
    {
        static GameManager manager;
        return manager;
    }

    void Init(sf::RenderWindow& _window);
    void Render(sf::RenderWindow& window);
    bool CollidesWithMap(const sf::FloatRect& bounds) const;

    void HandleEvent(const sf::Event& event, sf::RenderWindow& window);

    inline void SetJson(std::string _json) { json = _json; }

    inline std::vector<sf::Vector2f> GetSpawnPositions() { return spawnPositions; }
    inline Player* GetReferencePlayer() { return referencePlayer; }
    inline Player* GetEnemyPlayer() { return enemyPlayer; }

    inline void SetReferencePlayer(Player* player) { referencePlayer = player; }
    inline void SetEnemyPlayer(Player* player) { enemyPlayer = player; }
};
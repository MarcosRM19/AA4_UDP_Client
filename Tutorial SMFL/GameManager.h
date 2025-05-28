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

    GameManager() = default;
    GameManager(const GameManager&) = delete;
    GameManager& operator =(const GameManager&) = delete;

    int ExtractInt(const std::string& json, const std::string& key);
    std::vector<int> ExtractTileData(const std::string& json);
    
    void InitMap();

public:
    inline static GameManager& Instance()
    {
        static GameManager manager;
        return manager;
    }

    void Init(sf::RenderWindow& _window);
    void Update(sf::RenderWindow& window, const sf::Event& event);

    void HandleEvent(const sf::Event& event, sf::RenderWindow& window);

};
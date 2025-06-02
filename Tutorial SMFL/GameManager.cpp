#include "GameManager.h"
#include "TimeManager.h"
#include "NetworkManager.h"
#include "EventManager.h"
#include <SFML/Graphics.hpp>

int GameManager::ExtractInt(const std::string& json, const std::string& key) {
    size_t pos = json.find(key);
    if (pos == std::string::npos) return -1;

    size_t start = json.find(":", pos) + 1;
    size_t end = json.find_first_of(",}", start);
    return std::stoi(json.substr(start, end - start));
}

// Extracts the data array of tile IDs from the layer
std::vector<int> GameManager::ExtractTileData(const std::string& json) {
    std::vector<int> data;
    size_t pos = json.find("\"data\"");

    if (pos == std::string::npos) return data;

    size_t start = json.find("[", pos) + 1;
    size_t end = json.find("]", start);
    std::string arrayStr = json.substr(start, end - start);

    std::stringstream ss(arrayStr);
    std::string value;

    while (std::getline(ss, value, ',')) {
        int tile = std::stoi(value);
        data.push_back(tile);
    }

    return data;
}

void GameManager::InitMap()
{
    width = ExtractInt(json, "\"width\"");
    height = ExtractInt(json, "\"height\"");
    tileWidth = ExtractInt(json, "\"tilewidth\"");
    tileHeight = ExtractInt(json, "\"tileheight\"");

    tileData = ExtractTileData(json);

    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            int index = row * width + col;
            if (tileData[index] > 0) {
                sf::FloatRect rect(
                    { static_cast<float>(col * tileWidth), static_cast<float>(row * tileHeight) },
                    { static_cast<float>(tileWidth), static_cast<float>(tileHeight) }
                );
                collisionRects.push_back(rect);
            }
        }
    }
}

void GameManager::Init(sf::RenderWindow& _window)
{
    spawnPositions.push_back({ _window.getSize().x / 3.f, _window.getSize().y / 4.f});
    spawnPositions.push_back({ _window.getSize().x * 2.f / 3.f, _window.getSize().y / 4.f});
    spawnPositions.push_back({ _window.getSize().x / 2.f, _window.getSize().y / 2.f}); //respawn de cuando mueres

    InitMap();

    //Si no existe el tileset devuelve un error
    if (!tileset.loadFromFile("../Assets/Maps/platform.png")) {
        std::cerr << "Cannot load tileset.png\n";
        return;
    }
}

void GameManager::Render(sf::RenderWindow& window)
{
    int tileRows = tileset.getSize().x / tileWidth;
    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            int index = row * width + col;
            int tileID = tileData[index];
            if (tileID == 0) continue;

            tileID -= 1; // Tiled starts from 1

            int tx = tileID % tileRows;
            int ty = tileID / tileRows;

            sf::Sprite tile(tileset);
            tile.setTextureRect(sf::IntRect({ tx * tileWidth, ty * tileHeight }, { tileWidth, tileHeight }));
            tile.setPosition(sf::Vector2f(col * tileWidth, row * tileHeight));

            window.draw(tile);
        }
    }
}

void GameManager::HandleEvent(const sf::Event& event, sf::RenderWindow& window)
{
    if (event.is<sf::Event::Closed>())
    {
        window.close();
        CustomTCPPacket customPacket(DISCONNECT);
        EVENT_MANAGER.TCPEmit(DISCONNECT, customPacket);
    }
}

bool GameManager::CollidesWithMap(const sf::FloatRect& bounds) const {
    for (const sf::FloatRect& rect : collisionRects) {
        if (rect.findIntersection(bounds).has_value()) {
            return true;
        }
    }
    return false;
}

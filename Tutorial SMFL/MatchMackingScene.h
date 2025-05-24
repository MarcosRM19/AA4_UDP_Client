#pragma once
#include "Scene.h"

class MatchMackingScene : public Scene
{
private:
    std::vector<sf::RectangleShape> buttons;
    std::vector<sf::RectangleShape> textBackGround;
    std::vector<sf::Text> buttonsTexts;
    std::vector<sf::Text> idInformation;

public:
    MatchMackingScene();
    void Enter(sf::RenderWindow& window) override;
    void Exit() override;
    void Update(sf::RenderWindow& window, const sf::Event& event) override;
    void DetectRectangle(sf::Vector2f mousePosition) override;
    void CreateButtons(sf::RenderWindow& window, int id);
    void CreateTextField(sf::RenderWindow& window);
    void Render(sf::RenderWindow& window);
};


#pragma once
#include "Scene.h"

class MatchMackingScene : public Scene
{
private:
    std::vector<sf::RectangleShape> buttons;
    std::vector<sf::Text> buttonsTexts;

    std::vector<sf::RectangleShape> cancelButtons;
    std::vector<sf::Text> cancelButtonsTexts;

    bool searchingGame; 

public:
    MatchMackingScene();
    void Enter(sf::RenderWindow& window) override;
    void Exit() override;
    void Update(sf::RenderWindow& window, const sf::Event& event) override;
    void DetectRectangle(sf::Vector2f mousePosition) override;
    void CreateButtons(sf::RenderWindow& window);
    void CreateCancelButtons(sf::RenderWindow& window);
    void Render(sf::RenderWindow& window);
};


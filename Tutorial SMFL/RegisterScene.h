#pragma once
#include "Scene.h"

class RegisterScene : public Scene
{
private:
    std::vector<sf::RectangleShape> buttons;
    std::vector<sf::RectangleShape> textBackGround;
    std::vector<sf::Text> buttonsTexts;
    std::vector<sf::Text> textsInformation;
    std::vector<sf::Text> texts;

public:
    RegisterScene();
    void Enter(sf::RenderWindow& window) override;
    void Exit() override;
    void Update() override;
    void DetectRectangle(sf::Vector2f mousePosition) override;
    void CreateButtons(sf::RenderWindow& window, int id);
    void CreateTextField(sf::RenderWindow& window, int id);
    void Render(sf::RenderWindow& window) override;
    bool EmptyInformation();

    void SetCurrentPlayer(int id) override;
};


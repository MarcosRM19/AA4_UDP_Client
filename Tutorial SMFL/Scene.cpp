#include "Scene.h"
#include "SceneManager.h"

Scene::Scene()
{
	currentText = new sf::Text(SCENE.GetFont());
}

Scene::~Scene()
{
	delete currentText;
}

void Scene::HandleEvent(sf::RenderWindow& window, const sf::Event& event)
{
	if (event.is<sf::Event::Closed>())
		window.close();

	if (const sf::Event::MouseButtonPressed* mousePressed = event.getIf<sf::Event::MouseButtonPressed>())
	{
		switch (mousePressed->button)
		{
		case sf::Mouse::Button::Left:
			sf::Vector2f mousePososition = window.mapPixelToCoords(sf::Mouse::getPosition(window));
			DetectRectangle(mousePososition);
			break;
		}
	}

	if (const sf::Event::TextEntered* text = event.getIf<sf::Event::TextEntered>())
	{
		if (currentText == nullptr)
			return;

		char c = static_cast<char>(text->unicode);
		if (inputText.size() < maxLengthName && text->unicode != 8)
			inputText += c;

		currentText->setString(inputText);
	}

	if (const sf::Event::KeyPressed* keyPressed = event.getIf<sf::Event::KeyPressed>())
	{
		switch (keyPressed->code)
		{
		case sf::Keyboard::Key::Backspace:
			if (!inputText.empty())
				inputText.pop_back();
			break;
		}
	}
}

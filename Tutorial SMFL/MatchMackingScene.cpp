#include "MatchMackingScene.h"
#include "SceneManager.h"
#include "NetworkManager.h"
#include "EventManager.h"

MatchMackingScene::MatchMackingScene() : Scene()
{
}

void MatchMackingScene::Enter(sf::RenderWindow& window)
{
	std::cout << "Enter MatchMaking Scene" << std::endl;

	searchingGame = false;

	CreateCancelButtons(window);

	for (int i = 0; i < 2; i++)
	{
		CreateButtons(window, i);
	}
}

void MatchMackingScene::Exit()
{
	std::cout << "Exit MatchMaking Scene" << std::endl;
}

void MatchMackingScene::Update(sf::RenderWindow& window, const sf::Event& event)
{
	HandleEvent(window, event);
	Render(window);
}

void MatchMackingScene::DetectRectangle(sf::Vector2f mousePosition)
{
	for (size_t i = 0; i < cancelButtons.size(); i++) {
		if (cancelButtons[i].getGlobalBounds().contains(mousePosition) && searchingGame) {

			CustomPacket customPacket(CANCEL_QUEUE);

			std::cout << "Cancel Queue" << std::endl;
			//EVENT_MANAGER.Emit(customPacket.type, customPacket);
			searchingGame = false;
		}
	}

	for (size_t i = 0; i < buttons.size(); i++) {
		if (buttons[i].getGlobalBounds().contains(mousePosition) && !searchingGame) {

			if (i == 0)
			{
				//Mandar paquete de que buscamos partida amistosa
				CustomPacket customPacket(START_FRENDLY_QUEUE);

				std::cout << "Searching Friendly Game" << std::endl;
			}
			else if (i == 1)
			{
				//Mandar paquete de que buscamos partida amistosa
				CustomPacket customPacket(START_RANKED_QUEUE);

				std::cout << "Searching Ranked Game" << std::endl;

			}

			//EVENT_MANAGER.Emit(customPacket.type, customPacket);
			searchingGame = true;
		}
	}
}


void MatchMackingScene::CreateButtons(sf::RenderWindow& window, int id)
{
	buttons.push_back(sf::RectangleShape());
	buttonsTexts.push_back(sf::Text(SCENE.GetFont()));

	sf::Vector2f position(
		window.getSize().x / 2.f,
		window.getSize().y / 2.f + 200.f
	);
	std::string text;

	if (id == 0)
	{
		position.x -= 200;
		text = "Join Friendly Game";
	}
	else
	{
		position.x += 200;
		text = "Join Ranked Game";
	}

	buttons[id].setSize({ 300.f, 75.f });
	buttons[id].setFillColor(sf::Color::Green);
	buttons[id].setOrigin(buttons[id].getSize() / 2.f);
	buttons[id].setPosition(position);

	buttonsTexts[id].setString(text);
	buttonsTexts[id].setFillColor(sf::Color::Black);

	sf::FloatRect textBounds = buttonsTexts[id].getLocalBounds();
	buttonsTexts[id].setOrigin(sf::Vector2f(
		textBounds.position.x + textBounds.size.x / 2.f,
		textBounds.position.y + textBounds.size.y / 2.f
	));

	buttonsTexts[id].setPosition(position);
}

void MatchMackingScene::CreateCancelButtons(sf::RenderWindow& window)
{
	cancelButtons.push_back(sf::RectangleShape());
	cancelButtonsTexts.push_back(sf::Text(SCENE.GetFont()));

	sf::Vector2f position(
		window.getSize().x / 2.f,
		window.getSize().y / 2.f + 300.f
	);

	std::string text;
	text = "Cancel Queue";

	cancelButtons[0].setSize({ 300.f, 75.f });
	cancelButtons[0].setFillColor(sf::Color::Green);
	cancelButtons[0].setOrigin(cancelButtons[0].getSize() / 2.f);
	cancelButtons[0].setPosition(position);

	cancelButtonsTexts[0].setString(text);
	cancelButtonsTexts[0].setFillColor(sf::Color::Black);

	sf::FloatRect textBounds = cancelButtonsTexts[0].getLocalBounds();
	cancelButtonsTexts[0].setOrigin(sf::Vector2f(
		textBounds.position.x + textBounds.size.x / 2.f,
		textBounds.position.y + textBounds.size.y / 2.f
	));

	cancelButtonsTexts[0].setPosition(position);
}

void MatchMackingScene::Render(sf::RenderWindow& window)
{
	sf::Color brown(238, 208, 157);

	window.clear(brown);

	if (searchingGame)
	{
		for (sf::RectangleShape buton : cancelButtons)
			window.draw(buton);
		for (sf::Text text : cancelButtonsTexts)
			window.draw(text);
	}
	else
	{
		for (sf::RectangleShape buton : buttons)
			window.draw(buton);
		for (sf::Text text : buttonsTexts)
			window.draw(text);
	}

	window.display();
}
